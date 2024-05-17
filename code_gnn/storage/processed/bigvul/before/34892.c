call_transmit_status(struct rpc_task *task)
{
	task->tk_action = call_status;

	 
	if (task->tk_status == 0) {
		xprt_end_transmit(task);
		rpc_task_force_reencode(task);
		return;
	}

	switch (task->tk_status) {
	case -EAGAIN:
		break;
	default:
		dprint_status(task);
		xprt_end_transmit(task);
		rpc_task_force_reencode(task);
		break;
		 
	case -ECONNREFUSED:
	case -EHOSTDOWN:
	case -EHOSTUNREACH:
	case -ENETUNREACH:
		if (RPC_IS_SOFTCONN(task)) {
			xprt_end_transmit(task);
			rpc_exit(task, task->tk_status);
			break;
		}
	case -ECONNRESET:
	case -ENOTCONN:
	case -EPIPE:
		rpc_task_force_reencode(task);
	}
}
