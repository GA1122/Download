static void nfs41_sequence_free_slot(struct nfs4_sequence_res *res)
{
	struct nfs4_session *session;
	struct nfs4_slot_table *tbl;
	struct nfs4_slot *slot = res->sr_slot;
	bool send_new_highest_used_slotid = false;

	tbl = slot->table;
	session = tbl->session;

	spin_lock(&tbl->slot_tbl_lock);
	 
	if (tbl->highest_used_slotid > tbl->target_highest_slotid)
		send_new_highest_used_slotid = true;

	if (nfs41_wake_and_assign_slot(tbl, slot)) {
		send_new_highest_used_slotid = false;
		goto out_unlock;
	}
	nfs4_free_slot(tbl, slot);

	if (tbl->highest_used_slotid != NFS4_NO_SLOT)
		send_new_highest_used_slotid = false;
out_unlock:
	spin_unlock(&tbl->slot_tbl_lock);
	res->sr_slot = NULL;
	if (send_new_highest_used_slotid)
		nfs41_server_notify_highest_slotid_update(session->clp);
}