search_memslots(struct kvm_memslots *slots, gfn_t gfn)
{
	struct kvm_memory_slot *memslot;

	kvm_for_each_memslot(memslot, slots)
		if (gfn >= memslot->base_gfn &&
		      gfn < memslot->base_gfn + memslot->npages)
			return memslot;

	return NULL;
}
