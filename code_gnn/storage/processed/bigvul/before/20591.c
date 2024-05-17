int kvm_arch_prepare_memory_region(struct kvm *kvm,
		struct kvm_memory_slot *memslot,
		struct kvm_memory_slot old,
		struct kvm_userspace_memory_region *mem,
		int user_alloc)
{
	unsigned long i;
	unsigned long pfn;
	int npages = memslot->npages;
	unsigned long base_gfn = memslot->base_gfn;

	if (base_gfn + npages > (KVM_MAX_MEM_SIZE >> PAGE_SHIFT))
		return -ENOMEM;

	for (i = 0; i < npages; i++) {
		pfn = gfn_to_pfn(kvm, base_gfn + i);
		if (!kvm_is_mmio_pfn(pfn)) {
			kvm_set_pmt_entry(kvm, base_gfn + i,
					pfn << PAGE_SHIFT,
				_PAGE_AR_RWX | _PAGE_MA_WB);
			memslot->rmap[i] = (unsigned long)pfn_to_page(pfn);
		} else {
			kvm_set_pmt_entry(kvm, base_gfn + i,
					GPFN_PHYS_MMIO | (pfn << PAGE_SHIFT),
					_PAGE_MA_UC);
			memslot->rmap[i] = 0;
			}
	}

	return 0;
}
