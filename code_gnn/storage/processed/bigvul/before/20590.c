int kvm_arch_init_vm(struct kvm *kvm)
{
	BUG_ON(!kvm);

	kvm->arch.is_sn2 = ia64_platform_is("sn2");

	kvm->arch.metaphysical_rr0 = GUEST_PHYSICAL_RR0;
	kvm->arch.metaphysical_rr4 = GUEST_PHYSICAL_RR4;
	kvm->arch.vmm_init_rr = VMM_INIT_RR;

	 
	kvm_build_io_pmt(kvm);

	INIT_LIST_HEAD(&kvm->arch.assigned_dev_head);

	 
	set_bit(KVM_USERSPACE_IRQ_SOURCE_ID, &kvm->arch.irq_sources_bitmap);

	return 0;
}
