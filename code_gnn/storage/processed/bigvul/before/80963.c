static int init_rmode_identity_map(struct kvm *kvm)
{
	struct kvm_vmx *kvm_vmx = to_kvm_vmx(kvm);
	int i, idx, r = 0;
	kvm_pfn_t identity_map_pfn;
	u32 tmp;

	 
	mutex_lock(&kvm->slots_lock);

	if (likely(kvm_vmx->ept_identity_pagetable_done))
		goto out2;

	if (!kvm_vmx->ept_identity_map_addr)
		kvm_vmx->ept_identity_map_addr = VMX_EPT_IDENTITY_PAGETABLE_ADDR;
	identity_map_pfn = kvm_vmx->ept_identity_map_addr >> PAGE_SHIFT;

	r = __x86_set_memory_region(kvm, IDENTITY_PAGETABLE_PRIVATE_MEMSLOT,
				    kvm_vmx->ept_identity_map_addr, PAGE_SIZE);
	if (r < 0)
		goto out2;

	idx = srcu_read_lock(&kvm->srcu);
	r = kvm_clear_guest_page(kvm, identity_map_pfn, 0, PAGE_SIZE);
	if (r < 0)
		goto out;
	 
	for (i = 0; i < PT32_ENT_PER_PAGE; i++) {
		tmp = (i << 22) + (_PAGE_PRESENT | _PAGE_RW | _PAGE_USER |
			_PAGE_ACCESSED | _PAGE_DIRTY | _PAGE_PSE);
		r = kvm_write_guest_page(kvm, identity_map_pfn,
				&tmp, i * sizeof(tmp), sizeof(tmp));
		if (r < 0)
			goto out;
	}
	kvm_vmx->ept_identity_pagetable_done = true;

out:
	srcu_read_unlock(&kvm->srcu, idx);

out2:
	mutex_unlock(&kvm->slots_lock);
	return r;
}