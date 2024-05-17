nfsd4_cb_notify_lock_done(struct nfsd4_callback *cb, struct rpc_task *task)
{
	 
	switch (task->tk_status) {
	case -NFS4ERR_DELAY:
		rpc_delay(task, 1 * HZ);
		return 0;
	default:
		return 1;
	}
}
