static int nfsd4_cb_recall_done(struct nfsd4_callback *cb,
		struct rpc_task *task)
{
	struct nfs4_delegation *dp = cb_to_delegation(cb);

	if (dp->dl_stid.sc_type == NFS4_CLOSED_DELEG_STID)
	        return 1;

	switch (task->tk_status) {
	case 0:
		return 1;
	case -EBADHANDLE:
	case -NFS4ERR_BAD_STATEID:
		 
		if (dp->dl_retries--) {
			rpc_delay(task, 2 * HZ);
			return 0;
		}
		 
	default:
		return -1;
	}
}