int nfs41_sequence_done(struct rpc_task *task, struct nfs4_sequence_res *res)
{
	struct nfs4_session *session;
	struct nfs4_slot *slot = res->sr_slot;
	struct nfs_client *clp;
	bool interrupted = false;
	int ret = 1;

	if (slot == NULL)
		goto out_noaction;
	 
	if (!RPC_WAS_SENT(task))
		goto out;

	session = slot->table->session;

	if (slot->interrupted) {
		slot->interrupted = 0;
		interrupted = true;
	}

	trace_nfs4_sequence_done(session, res);
	 
	switch (res->sr_status) {
	case 0:
		 
		++slot->seq_nr;
		clp = session->clp;
		do_renew_lease(clp, res->sr_timestamp);
		 
		nfs41_handle_sequence_flag_errors(clp, res->sr_status_flags);
		nfs41_update_target_slotid(slot->table, slot, res);
		break;
	case 1:
		 
		slot->interrupted = 1;
		goto out;
	case -NFS4ERR_DELAY:
		 
		dprintk("%s: slot=%u seq=%u: Operation in progress\n",
			__func__,
			slot->slot_nr,
			slot->seq_nr);
		goto out_retry;
	case -NFS4ERR_BADSLOT:
		 
		goto retry_nowait;
	case -NFS4ERR_SEQ_MISORDERED:
		 
		if (interrupted) {
			++slot->seq_nr;
			goto retry_nowait;
		}
		 
		if (slot->seq_nr != 1) {
			slot->seq_nr = 1;
			goto retry_nowait;
		}
		break;
	case -NFS4ERR_SEQ_FALSE_RETRY:
		++slot->seq_nr;
		goto retry_nowait;
	default:
		 
		++slot->seq_nr;
	}
out:
	 
	dprintk("%s: Error %d free the slot \n", __func__, res->sr_status);
	nfs41_sequence_free_slot(res);
out_noaction:
	return ret;
retry_nowait:
	if (rpc_restart_call_prepare(task)) {
		task->tk_status = 0;
		ret = 0;
	}
	goto out;
out_retry:
	if (!rpc_restart_call(task))
		goto out;
	rpc_delay(task, NFS4_POLL_RETRY_MAX);
	return 0;
}
