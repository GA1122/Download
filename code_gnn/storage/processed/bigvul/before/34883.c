call_decode(struct rpc_task *task)
{
	struct rpc_clnt	*clnt = task->tk_client;
	struct rpc_rqst	*req = task->tk_rqstp;
	kxdrdproc_t	decode = task->tk_msg.rpc_proc->p_decode;
	__be32		*p;

	dprintk("RPC: %5u call_decode (status %d)\n",
			task->tk_pid, task->tk_status);

	if (task->tk_flags & RPC_CALL_MAJORSEEN) {
		if (clnt->cl_chatty)
			printk(KERN_NOTICE "%s: server %s OK\n",
				clnt->cl_protname, clnt->cl_server);
		task->tk_flags &= ~RPC_CALL_MAJORSEEN;
	}

	 
	smp_rmb();
	req->rq_rcv_buf.len = req->rq_private_buf.len;

	 
	WARN_ON(memcmp(&req->rq_rcv_buf, &req->rq_private_buf,
				sizeof(req->rq_rcv_buf)) != 0);

	if (req->rq_rcv_buf.len < 12) {
		if (!RPC_IS_SOFT(task)) {
			task->tk_action = call_bind;
			clnt->cl_stats->rpcretrans++;
			goto out_retry;
		}
		dprintk("RPC:       %s: too small RPC reply size (%d bytes)\n",
				clnt->cl_protname, task->tk_status);
		task->tk_action = call_timeout;
		goto out_retry;
	}

	p = rpc_verify_header(task);
	if (IS_ERR(p)) {
		if (p == ERR_PTR(-EAGAIN))
			goto out_retry;
		return;
	}

	task->tk_action = rpc_exit_task;

	if (decode) {
		task->tk_status = rpcauth_unwrap_resp(task, decode, req, p,
						      task->tk_msg.rpc_resp);
	}
	dprintk("RPC: %5u call_decode result %d\n", task->tk_pid,
			task->tk_status);
	return;
out_retry:
	task->tk_status = 0;
	 
	if (task->tk_rqstp == req) {
		req->rq_reply_bytes_recvd = req->rq_rcv_buf.len = 0;
		if (task->tk_client->cl_discrtry)
			xprt_conditional_disconnect(task->tk_xprt,
					req->rq_connect_cookie);
	}
}