static int __apic_accept_irq(struct kvm_vcpu *vcpu, uint64_t vector)
{
	struct vpd *vpd = to_host(vcpu->kvm, vcpu->arch.vpd);

	if (!test_and_set_bit(vector, &vpd->irr[0])) {
		vcpu->arch.irq_new_pending = 1;
		kvm_vcpu_kick(vcpu);
		return 1;
	}
	return 0;
}
