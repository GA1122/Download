static int kvm_vcpu_first_run_init(struct kvm_vcpu *vcpu)
{
	if (likely(vcpu->arch.has_run_once))
		return 0;

	vcpu->arch.has_run_once = true;

	 
	if (irqchip_in_kernel(vcpu->kvm) &&
	    unlikely(!vgic_initialized(vcpu->kvm))) {
		int ret = kvm_vgic_init(vcpu->kvm);
		if (ret)
			return ret;
	}

	 
	if (test_and_clear_bit(KVM_ARM_VCPU_POWER_OFF, vcpu->arch.features)) {
		*vcpu_reg(vcpu, 0) = KVM_PSCI_FN_CPU_OFF;
		kvm_psci_call(vcpu);
	}

	return 0;
}
