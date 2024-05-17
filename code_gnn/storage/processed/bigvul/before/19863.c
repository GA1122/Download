static int nfs41_sequence_done(struct rpc_task *task, struct nfs4_sequence_res *res)
{
	unsigned long timestamp;
	struct nfs_client *clp;

	 
	if (res->sr_status == 1)
		res->sr_status = NFS_OK;

	 
	if (!RPC_WAS_SENT(task))
		goto out;

	 
	switch (res->sr_status) {
	case 0:
		 
		++res->sr_slot->seq_nr;
		timestamp = res->sr_renewal_time;
		clp = res->sr_session->clp;
		do_renew_lease(clp, timestamp);
		 
		if (res->sr_status_flags != 0)
			nfs4_schedule_lease_recovery(clp);
		break;
	case -NFS4ERR_DELAY:
		 
		dprintk("%s: slot=%td seq=%d: Operation in progress\n",
			__func__,
			res->sr_slot - res->sr_session->fc_slot_table.slots,
			res->sr_slot->seq_nr);
		goto out_retry;
	default:
		 
		++res->sr_slot->seq_nr;
	}
out:
	 
	dprintk("%s: Error %d free the slot \n", __func__, res->sr_status);
	nfs41_sequence_free_slot(res);
	return 1;
out_retry:
	if (!rpc_restart_call(task))
		goto out;
	rpc_delay(task, NFS4_POLL_RETRY_MAX);
	return 0;
}