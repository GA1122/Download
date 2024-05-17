void kvm_arch_mmu_notifier_invalidate_page(struct kvm *kvm,
					   unsigned long address)
{
	 
	if (address == gfn_to_hva(kvm, APIC_DEFAULT_PHYS_BASE >> PAGE_SHIFT))
		kvm_make_all_cpus_request(kvm, KVM_REQ_APIC_PAGE_RELOAD);
}
