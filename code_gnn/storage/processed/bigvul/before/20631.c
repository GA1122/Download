static void kvm_release_vm_pages(struct kvm *kvm)
{
	struct kvm_memslots *slots;
	struct kvm_memory_slot *memslot;
	int j;
	unsigned long base_gfn;

	slots = kvm_memslots(kvm);
	kvm_for_each_memslot(memslot, slots) {
		base_gfn = memslot->base_gfn;
		for (j = 0; j < memslot->npages; j++) {
			if (memslot->rmap[j])
				put_page((struct page *)memslot->rmap[j]);
		}
	}
}
