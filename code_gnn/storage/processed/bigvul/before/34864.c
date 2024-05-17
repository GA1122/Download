static void nlmclnt_cancel_callback(struct rpc_task *task, void *data)
{
	struct nlm_rqst	*req = data;
	u32 status = ntohl(req->a_res.status);

	if (RPC_ASSASSINATED(task))
		goto die;

	if (task->tk_status < 0) {
		dprintk("lockd: CANCEL call error %d, retrying.\n",
					task->tk_status);
		goto retry_cancel;
	}

	dprintk("lockd: cancel status %u (task %u)\n",
			status, task->tk_pid);

	switch (status) {
	case NLM_LCK_GRANTED:
	case NLM_LCK_DENIED_GRACE_PERIOD:
	case NLM_LCK_DENIED:
		 
		break;
	case NLM_LCK_DENIED_NOLOCKS:
		dprintk("lockd: CANCEL failed (server has no locks)\n");
		goto retry_cancel;
	default:
		printk(KERN_NOTICE "lockd: weird return %d for CANCEL call\n",
			status);
	}

die:
	return;

retry_cancel:
	 
	if (req->a_retries++ >= NLMCLNT_MAX_RETRIES)
		goto die;
	nlm_rebind_host(req->a_host);
	rpc_restart_call(task);
	rpc_delay(task, 30 * HZ);
}