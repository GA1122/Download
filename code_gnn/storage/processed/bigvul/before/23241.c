static int nfs4_reset_slot_table(struct nfs4_slot_table *tbl, u32 max_reqs,
				 int ivalue)
{
	struct nfs4_slot *new = NULL;
	int i;
	int ret = 0;

	dprintk("--> %s: max_reqs=%u, tbl->max_slots %d\n", __func__,
		max_reqs, tbl->max_slots);

	 
	if (max_reqs != tbl->max_slots) {
		ret = -ENOMEM;
		new = kmalloc(max_reqs * sizeof(struct nfs4_slot),
			      GFP_NOFS);
		if (!new)
			goto out;
		ret = 0;
		kfree(tbl->slots);
	}
	spin_lock(&tbl->slot_tbl_lock);
	if (new) {
		tbl->slots = new;
		tbl->max_slots = max_reqs;
	}
	for (i = 0; i < tbl->max_slots; ++i)
		tbl->slots[i].seq_nr = ivalue;
	spin_unlock(&tbl->slot_tbl_lock);
	dprintk("%s: tbl=%p slots=%p max_slots=%d\n", __func__,
		tbl, tbl->slots, tbl->max_slots);
out:
	dprintk("<-- %s: return %d\n", __func__, ret);
	return ret;
}