static inline bool kvm_vcpu_trigger_posted_interrupt(struct kvm_vcpu *vcpu)
{
#ifdef CONFIG_SMP
	if (vcpu->mode == IN_GUEST_MODE) {
		struct vcpu_vmx *vmx = to_vmx(vcpu);

		 
		WARN_ON_ONCE(pi_test_sn(&vmx->pi_desc));

		apic->send_IPI_mask(get_cpu_mask(vcpu->cpu),
				POSTED_INTR_VECTOR);
		return true;
	}
#endif
	return false;
}
