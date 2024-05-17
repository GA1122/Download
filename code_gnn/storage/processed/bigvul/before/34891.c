call_transmit(struct rpc_task *task)
{
	dprint_status(task);

	task->tk_action = call_status;
	if (task->tk_status < 0)
		return;
	task->tk_status = xprt_prepare_transmit(task);
	if (task->tk_status != 0)
		return;
	task->tk_action = call_transmit_status;
	 
	if (rpc_task_need_encode(task)) {
		BUG_ON(task->tk_rqstp->rq_bytes_sent != 0);
		rpc_xdr_encode(task);
		 
		if (task->tk_status != 0) {
			 
			if (task->tk_status == -EAGAIN)
				rpc_delay(task, HZ >> 4);
			else
				rpc_exit(task, task->tk_status);
			return;
		}
	}
	xprt_transmit(task);
	if (task->tk_status < 0)
		return;
	 
	call_transmit_status(task);
	if (rpc_reply_expected(task))
		return;
	task->tk_action = rpc_exit_task;
	rpc_wake_up_queued_task(&task->tk_xprt->pending, task);
}
