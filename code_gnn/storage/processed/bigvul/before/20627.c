static int kvm_insert_vmm_mapping(struct kvm_vcpu *vcpu)
{
	unsigned long pte;
	struct kvm *kvm = vcpu->kvm;
	int r;

	 
	pte = pte_val(mk_pte_phys(__pa(kvm_vmm_base), PAGE_KERNEL));
	r = ia64_itr_entry(0x3, KVM_VMM_BASE, pte, KVM_VMM_SHIFT);
	if (r < 0)
		goto out;
	vcpu->arch.vmm_tr_slot = r;
	 
	pte = pte_val(mk_pte_phys(__pa(kvm->arch.vm_base), PAGE_KERNEL));
	r = ia64_itr_entry(0x3, KVM_VM_DATA_BASE,
					pte, KVM_VM_DATA_SHIFT);
	if (r < 0)
		goto out;
	vcpu->arch.vm_tr_slot = r;

#if defined(CONFIG_IA64_SGI_SN2) || defined(CONFIG_IA64_GENERIC)
	if (kvm->arch.is_sn2) {
		r = kvm_sn2_setup_mappings(vcpu);
		if (r < 0)
			goto out;
	}
#endif

	r = 0;
out:
	return r;
}
