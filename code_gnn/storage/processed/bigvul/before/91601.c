static void update_memslots(struct kvm_memslots *slots,
			    struct kvm_memory_slot *new,
			    enum kvm_mr_change change)
{
	int id = new->id;
	int i = slots->id_to_index[id];
	struct kvm_memory_slot *mslots = slots->memslots;

	WARN_ON(mslots[i].id != id);
	switch (change) {
	case KVM_MR_CREATE:
		slots->used_slots++;
		WARN_ON(mslots[i].npages || !new->npages);
		break;
	case KVM_MR_DELETE:
		slots->used_slots--;
		WARN_ON(new->npages || !mslots[i].npages);
		break;
	default:
		break;
	}

	while (i < KVM_MEM_SLOTS_NUM - 1 &&
	       new->base_gfn <= mslots[i + 1].base_gfn) {
		if (!mslots[i + 1].npages)
			break;
		mslots[i] = mslots[i + 1];
		slots->id_to_index[mslots[i].id] = i;
		i++;
	}

	 
	if (new->npages) {
		while (i > 0 &&
		       new->base_gfn >= mslots[i - 1].base_gfn) {
			mslots[i] = mslots[i - 1];
			slots->id_to_index[mslots[i].id] = i;
			i--;
		}
	} else
		WARN_ON_ONCE(i != slots->used_slots);

	mslots[i] = *new;
	slots->id_to_index[mslots[i].id] = i;
}
