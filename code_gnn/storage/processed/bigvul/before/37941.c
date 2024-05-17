void kvm_vcpu_reload_apic_access_page(struct kvm_vcpu *vcpu)
{
	struct page *page = NULL;

	if (!irqchip_in_kernel(vcpu->kvm))
		return;

	if (!kvm_x86_ops->set_apic_access_page_addr)
		return;

	page = gfn_to_page(vcpu->kvm, APIC_DEFAULT_PHYS_BASE >> PAGE_SHIFT);
	kvm_x86_ops->set_apic_access_page_addr(vcpu, page_to_phys(page));

	 
	put_page(page);
}
