static int apic_set_eoi(struct kvm_lapic *apic)
{
	int vector = apic_find_highest_isr(apic);

	trace_kvm_eoi(apic, vector);

	 
	if (vector == -1)
		return vector;

	apic_clear_isr(vector, apic);
	apic_update_ppr(apic);

	kvm_ioapic_send_eoi(apic, vector);
	kvm_make_request(KVM_REQ_EVENT, apic->vcpu);
	return vector;
}