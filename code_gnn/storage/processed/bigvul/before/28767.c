void kvm_free_lapic(struct kvm_vcpu *vcpu)
{
	struct kvm_lapic *apic = vcpu->arch.apic;

	if (!vcpu->arch.apic)
		return;

	hrtimer_cancel(&apic->lapic_timer.timer);

	if (!(vcpu->arch.apic_base & MSR_IA32_APICBASE_ENABLE))
		static_key_slow_dec_deferred(&apic_hw_disabled);

	if (!(kvm_apic_get_reg(apic, APIC_SPIV) & APIC_SPIV_APIC_ENABLED))
		static_key_slow_dec_deferred(&apic_sw_disabled);

	if (apic->regs)
		free_page((unsigned long)apic->regs);

	kfree(apic);
}