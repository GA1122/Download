static struct nfsd4_session *alloc_session(struct nfsd4_channel_attrs *fattrs,
					   struct nfsd4_channel_attrs *battrs)
{
	int numslots = fattrs->maxreqs;
	int slotsize = slot_bytes(fattrs);
	struct nfsd4_session *new;
	int mem, i;

	BUILD_BUG_ON(NFSD_MAX_SLOTS_PER_SESSION * sizeof(struct nfsd4_slot *)
			+ sizeof(struct nfsd4_session) > PAGE_SIZE);
	mem = numslots * sizeof(struct nfsd4_slot *);

	new = kzalloc(sizeof(*new) + mem, GFP_KERNEL);
	if (!new)
		return NULL;
	 
	for (i = 0; i < numslots; i++) {
		new->se_slots[i] = kzalloc(slotsize, GFP_KERNEL);
		if (!new->se_slots[i])
			goto out_free;
	}

	memcpy(&new->se_fchannel, fattrs, sizeof(struct nfsd4_channel_attrs));
	memcpy(&new->se_bchannel, battrs, sizeof(struct nfsd4_channel_attrs));

	return new;
out_free:
	while (i--)
		kfree(new->se_slots[i]);
	kfree(new);
	return NULL;
}