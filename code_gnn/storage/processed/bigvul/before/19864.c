static void nfs41_sequence_free_slot(struct nfs4_sequence_res *res)
{
	struct nfs4_slot_table *tbl;

	tbl = &res->sr_session->fc_slot_table;
	if (!res->sr_slot) {
		 
		dprintk("%s: No slot\n", __func__);
		return;
	}

	spin_lock(&tbl->slot_tbl_lock);
	nfs4_free_slot(tbl, res->sr_slot - tbl->slots);
	nfs4_check_drain_fc_complete(res->sr_session);
	spin_unlock(&tbl->slot_tbl_lock);
	res->sr_slot = NULL;
}
