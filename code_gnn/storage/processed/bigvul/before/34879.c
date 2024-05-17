call_bc_transmit(struct rpc_task *task)
{
	struct rpc_rqst *req = task->tk_rqstp;

	BUG_ON(task->tk_status != 0);
	task->tk_status = xprt_prepare_transmit(task);
	if (task->tk_status == -EAGAIN) {
		 
		task->tk_status = 0;
		task->tk_action = call_bc_transmit;
		return;
	}

	task->tk_action = rpc_exit_task;
	if (task->tk_status < 0) {
		printk(KERN_NOTICE "RPC: Could not send backchannel reply "
			"error: %d\n", task->tk_status);
		return;
	}

	xprt_transmit(task);
	xprt_end_transmit(task);
	dprint_status(task);
	switch (task->tk_status) {
	case 0:
		 
		break;
	case -EHOSTDOWN:
	case -EHOSTUNREACH:
	case -ENETUNREACH:
	case -ETIMEDOUT:
		 
		printk(KERN_NOTICE "RPC: Could not send backchannel reply "
			"error: %d\n", task->tk_status);
		xprt_conditional_disconnect(task->tk_xprt,
			req->rq_connect_cookie);
		break;
	default:
		 
		BUG_ON(task->tk_status == -EAGAIN);
		printk(KERN_NOTICE "RPC: Could not send backchannel reply "
			"error: %d\n", task->tk_status);
		break;
	}
	rpc_wake_up_queued_task(&req->rq_xprt->pending, task);
}
