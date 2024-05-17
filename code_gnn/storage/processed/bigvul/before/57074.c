int nfs40_setup_sequence(struct nfs4_slot_table *tbl,
			 struct nfs4_sequence_args *args,
			 struct nfs4_sequence_res *res,
			 struct rpc_task *task)
{
	struct nfs4_slot *slot;

	 
	if (res->sr_slot != NULL)
		goto out_start;

	spin_lock(&tbl->slot_tbl_lock);
	if (nfs4_slot_tbl_draining(tbl) && !args->sa_privileged)
		goto out_sleep;

	slot = nfs4_alloc_slot(tbl);
	if (IS_ERR(slot)) {
		if (slot == ERR_PTR(-ENOMEM))
			task->tk_timeout = HZ >> 2;
		goto out_sleep;
	}
	spin_unlock(&tbl->slot_tbl_lock);

	args->sa_slot = slot;
	res->sr_slot = slot;

out_start:
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