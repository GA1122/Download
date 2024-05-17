static struct kvm_memory_slot *__gfn_to_memslot(struct kvm_memslots *slots,
						gfn_t gfn)
{
	return search_memslots(slots, gfn);
}
