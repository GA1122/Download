int kvm_apic_match_physical_addr(struct kvm_lapic *apic, u16 dest)
{
	return apic->vcpu->vcpu_id == dest;
}
