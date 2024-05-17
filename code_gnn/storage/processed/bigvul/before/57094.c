int nfs41_setup_sequence(struct nfs4_session *session,
				struct nfs4_sequence_args *args,
				struct nfs4_sequence_res *res,
				struct rpc_task *task)
{
	struct nfs4_slot *slot;
	struct nfs4_slot_table *tbl;

	dprintk("--> %s\n", __func__);
	 
	if (res->sr_slot != NULL)
		goto out_success;

	tbl = &session->fc_slot_table;

	task->tk_timeout = 0;

	spin_lock(&tbl->slot_tbl_lock);
	if (test_bit(NFS4_SLOT_TBL_DRAINING, &tbl->slot_tbl_state) &&
	    !args->sa_privileged) {
		 
		dprintk("%s session is draining\n", __func__);
		goto out_sleep;
	}

	slot = nfs4_alloc_slot(tbl);
	if (IS_ERR(slot)) {
		 
		if (slot == ERR_PTR(-ENOMEM))
			task->tk_timeout = HZ >> 2;
		dprintk("<-- %s: no free slots\n", __func__);
		goto out_sleep;
	}
	spin_unlock(&tbl->slot_tbl_lock);

	args->sa_slot = slot;

	dprintk("<-- %s slotid=%u seqid=%u\n", __func__,
			slot->slot_nr, slot->seq_nr);

	res->sr_slot = slot;
	res->sr_timestamp = jiffies;
	res->sr_status_flags = 0;
	 
	res->sr_status = 1;
	trace_nfs4_setup_sequence(session, args);
out_success:
	rpc_call_start(task);
	return 0;
out_sleep:
	 
	if (args->sa_privileged)
		rpc_sleep_on_priority(&tbl->slot_tbl_waitq, task,
				NULL, RPC_PRIORITY_PRIVILEGED);
	else
		rpc_sleep_on(&tbl->slot_tbl_waitq, task, NULL);
	spin_unlock(&tbl->slot_tbl_lock);
	return -EAGAIN;
}