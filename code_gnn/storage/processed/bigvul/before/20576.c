int kvm_apic_set_irq(struct kvm_vcpu *vcpu, struct kvm_lapic_irq *irq)
{
	return __apic_accept_irq(vcpu, irq->vector);
}
