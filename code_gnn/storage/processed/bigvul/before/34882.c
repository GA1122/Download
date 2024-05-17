call_connect_status(struct rpc_task *task)
{
	struct rpc_clnt *clnt = task->tk_client;
	int status = task->tk_status;

	dprint_status(task);

	task->tk_status = 0;
	if (status >= 0 || status == -EAGAIN) {
		clnt->cl_stats->netreconn++;
		task->tk_action = call_transmit;
		return;
	}

	switch (status) {
		 
	case -ETIMEDOUT:
		task->tk_action = call_timeout;
		break;
	default:
		rpc_exit(task, -EIO);
	}
}