int kvm_arch_vcpu_runnable(struct kvm_vcpu *vcpu)
{
	return (vcpu->arch.mp_state == KVM_MP_STATE_RUNNABLE) ||
		(kvm_highest_pending_irq(vcpu) != -1);
}
