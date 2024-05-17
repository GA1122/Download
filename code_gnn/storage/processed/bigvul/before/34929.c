rpc_verify_header(struct rpc_task *task)
{
	struct kvec *iov = &task->tk_rqstp->rq_rcv_buf.head[0];
	int len = task->tk_rqstp->rq_rcv_buf.len >> 2;
	__be32	*p = iov->iov_base;
	u32 n;
	int error = -EACCES;

	if ((task->tk_rqstp->rq_rcv_buf.len & 3) != 0) {
		 
		dprintk("RPC: %5u %s: XDR representation not a multiple of"
		       " 4 bytes: 0x%x\n", task->tk_pid, __func__,
		       task->tk_rqstp->rq_rcv_buf.len);
		goto out_eio;
	}
	if ((len -= 3) < 0)
		goto out_overflow;

	p += 1;  
	if ((n = ntohl(*p++)) != RPC_REPLY) {
		dprintk("RPC: %5u %s: not an RPC reply: %x\n",
			task->tk_pid, __func__, n);
		goto out_garbage;
	}

	if ((n = ntohl(*p++)) != RPC_MSG_ACCEPTED) {
		if (--len < 0)
			goto out_overflow;
		switch ((n = ntohl(*p++))) {
			case RPC_AUTH_ERROR:
				break;
			case RPC_MISMATCH:
				dprintk("RPC: %5u %s: RPC call version "
						"mismatch!\n",
						task->tk_pid, __func__);
				error = -EPROTONOSUPPORT;
				goto out_err;
			default:
				dprintk("RPC: %5u %s: RPC call rejected, "
						"unknown error: %x\n",
						task->tk_pid, __func__, n);
				goto out_eio;
		}
		if (--len < 0)
			goto out_overflow;
		switch ((n = ntohl(*p++))) {
		case RPC_AUTH_REJECTEDCRED:
		case RPC_AUTH_REJECTEDVERF:
		case RPCSEC_GSS_CREDPROBLEM:
		case RPCSEC_GSS_CTXPROBLEM:
			if (!task->tk_cred_retry)
				break;
			task->tk_cred_retry--;
			dprintk("RPC: %5u %s: retry stale creds\n",
					task->tk_pid, __func__);
			rpcauth_invalcred(task);
			 
			xprt_release(task);
			task->tk_action = call_reserve;
			goto out_retry;
		case RPC_AUTH_BADCRED:
		case RPC_AUTH_BADVERF:
			 
			if (!task->tk_garb_retry)
				break;
			task->tk_garb_retry--;
			dprintk("RPC: %5u %s: retry garbled creds\n",
					task->tk_pid, __func__);
			task->tk_action = call_bind;
			goto out_retry;
		case RPC_AUTH_TOOWEAK:
			printk(KERN_NOTICE "RPC: server %s requires stronger "
			       "authentication.\n", task->tk_client->cl_server);
			break;
		default:
			dprintk("RPC: %5u %s: unknown auth error: %x\n",
					task->tk_pid, __func__, n);
			error = -EIO;
		}
		dprintk("RPC: %5u %s: call rejected %d\n",
				task->tk_pid, __func__, n);
		goto out_err;
	}
	if (!(p = rpcauth_checkverf(task, p))) {
		dprintk("RPC: %5u %s: auth check failed\n",
				task->tk_pid, __func__);
		goto out_garbage;		 
	}
	len = p - (__be32 *)iov->iov_base - 1;
	if (len < 0)
		goto out_overflow;
	switch ((n = ntohl(*p++))) {
	case RPC_SUCCESS:
		return p;
	case RPC_PROG_UNAVAIL:
		dprintk("RPC: %5u %s: program %u is unsupported by server %s\n",
				task->tk_pid, __func__,
				(unsigned int)task->tk_client->cl_prog,
				task->tk_client->cl_server);
		error = -EPFNOSUPPORT;
		goto out_err;
	case RPC_PROG_MISMATCH:
		dprintk("RPC: %5u %s: program %u, version %u unsupported by "
				"server %s\n", task->tk_pid, __func__,
				(unsigned int)task->tk_client->cl_prog,
				(unsigned int)task->tk_client->cl_vers,
				task->tk_client->cl_server);
		error = -EPROTONOSUPPORT;
		goto out_err;
	case RPC_PROC_UNAVAIL:
		dprintk("RPC: %5u %s: proc %s unsupported by program %u, "
				"version %u on server %s\n",
				task->tk_pid, __func__,
				rpc_proc_name(task),
				task->tk_client->cl_prog,
				task->tk_client->cl_vers,
				task->tk_client->cl_server);
		error = -EOPNOTSUPP;
		goto out_err;
	case RPC_GARBAGE_ARGS:
		dprintk("RPC: %5u %s: server saw garbage\n",
				task->tk_pid, __func__);
		break;			 
	default:
		dprintk("RPC: %5u %s: server accept status: %x\n",
				task->tk_pid, __func__, n);
		 
	}

out_garbage:
	task->tk_client->cl_stats->rpcgarbage++;
	if (task->tk_garb_retry) {
		task->tk_garb_retry--;
		dprintk("RPC: %5u %s: retrying\n",
				task->tk_pid, __func__);
		task->tk_action = call_bind;
out_retry:
		return ERR_PTR(-EAGAIN);
	}
out_eio:
	error = -EIO;
out_err:
	rpc_exit(task, error);
	dprintk("RPC: %5u %s: call failed with error %d\n", task->tk_pid,
			__func__, error);
	return ERR_PTR(error);
out_overflow:
	dprintk("RPC: %5u %s: server reply was truncated.\n", task->tk_pid,
			__func__);
	goto out_garbage;
}