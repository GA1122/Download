call_status(struct rpc_task *task)
{
	struct rpc_clnt	*clnt = task->tk_client;
	struct rpc_rqst	*req = task->tk_rqstp;
	int		status;

	if (req->rq_reply_bytes_recvd > 0 && !req->rq_bytes_sent)
		task->tk_status = req->rq_reply_bytes_recvd;

	dprint_status(task);

	status = task->tk_status;
	if (status >= 0) {
		task->tk_action = call_decode;
		return;
	}

	task->tk_status = 0;
	switch(status) {
	case -EHOSTDOWN:
	case -EHOSTUNREACH:
	case -ENETUNREACH:
		 
		rpc_delay(task, 3*HZ);
	case -ETIMEDOUT:
		task->tk_action = call_timeout;
		if (task->tk_client->cl_discrtry)
			xprt_conditional_disconnect(task->tk_xprt,
					req->rq_connect_cookie);
		break;
	case -ECONNRESET:
	case -ECONNREFUSED:
		rpc_force_rebind(clnt);
		rpc_delay(task, 3*HZ);
	case -EPIPE:
	case -ENOTCONN:
		task->tk_action = call_bind;
		break;
	case -EAGAIN:
		task->tk_action = call_transmit;
		break;
	case -EIO:
		 
		rpc_exit(task, status);
		break;
	default:
		if (clnt->cl_chatty)
			printk("%s: RPC call returned error %d\n",
			       clnt->cl_protname, -status);
		rpc_exit(task, status);
	}
}