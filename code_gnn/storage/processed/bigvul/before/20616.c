int kvm_cpu_has_pending_timer(struct kvm_vcpu *vcpu)
{
	return vcpu->arch.timer_fired;
}
