static int handle_apic_eoi_induced(struct kvm_vcpu *vcpu)
{
	unsigned long exit_qualification = vmcs_readl(EXIT_QUALIFICATION);
	int vector = exit_qualification & 0xff;

	 
	kvm_apic_set_eoi_accelerated(vcpu, vector);
	return 1;
}
