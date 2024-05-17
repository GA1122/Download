call_reserveresult(struct rpc_task *task)
{
	int status = task->tk_status;

	dprint_status(task);

	 
	task->tk_status = 0;
	if (status >= 0) {
		if (task->tk_rqstp) {
			task->tk_action = call_refresh;
			return;
		}

		printk(KERN_ERR "%s: status=%d, but no request slot, exiting\n",
				__func__, status);
		rpc_exit(task, -EIO);
		return;
	}

	 
	if (task->tk_rqstp) {
		printk(KERN_ERR "%s: status=%d, request allocated anyway\n",
				__func__, status);
		xprt_release(task);
	}

	switch (status) {
	case -EAGAIN:	 
		task->tk_action = call_reserve;
		return;
	case -EIO:	 
		break;
	default:
		printk(KERN_ERR "%s: unrecognized error %d, exiting\n",
				__func__, status);
		break;
	}
	rpc_exit(task, status);
}
