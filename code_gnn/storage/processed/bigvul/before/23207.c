static int nfs4_init_slot_table(struct nfs4_slot_table *tbl,
		int max_slots, int ivalue)
{
	struct nfs4_slot *slot;
	int ret = -ENOMEM;

	BUG_ON(max_slots > NFS4_MAX_SLOT_TABLE);

	dprintk("--> %s: max_reqs=%u\n", __func__, max_slots);

	slot = kcalloc(max_slots, sizeof(struct nfs4_slot), GFP_NOFS);
	if (!slot)
		goto out;
	ret = 0;

	spin_lock(&tbl->slot_tbl_lock);
	tbl->max_slots = max_slots;
	tbl->slots = slot;
	tbl->highest_used_slotid = -1;   
	spin_unlock(&tbl->slot_tbl_lock);
	dprintk("%s: tbl=%p slots=%p max_slots=%d\n", __func__,
		tbl, tbl->slots, tbl->max_slots);
out:
	dprintk("<-- %s: return %d\n", __func__, ret);
	return ret;
}