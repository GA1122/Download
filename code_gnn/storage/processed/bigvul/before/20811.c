void kvm_set_apic_base(struct kvm_vcpu *vcpu, u64 data)
{
	 
	if (irqchip_in_kernel(vcpu->kvm))
		kvm_lapic_set_base(vcpu, data);
	else
		vcpu->arch.apic_base = data;
}
