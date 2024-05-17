int kvm_apic_match_dest(struct kvm_vcpu *vcpu, struct kvm_lapic *source,
		int short_hand, int dest, int dest_mode)
{
	struct kvm_lapic *target = vcpu->arch.apic;
	return (dest_mode == 0) ?
		kvm_apic_match_physical_addr(target, dest) :
		kvm_apic_match_logical_addr(target, dest);
}
