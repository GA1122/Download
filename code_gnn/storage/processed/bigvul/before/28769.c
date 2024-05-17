u64 kvm_get_lapic_tscdeadline_msr(struct kvm_vcpu *vcpu)
{
	struct kvm_lapic *apic = vcpu->arch.apic;

	if (!kvm_vcpu_has_lapic(vcpu) || apic_lvtt_oneshot(apic) ||
			apic_lvtt_period(apic))
		return 0;

	return apic->lapic_timer.tscdeadline;
}