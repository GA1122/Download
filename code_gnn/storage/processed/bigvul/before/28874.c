void kvm_set_apic_base(struct kvm_vcpu *vcpu, u64 data)
{
	 
	kvm_lapic_set_base(vcpu, data);
}