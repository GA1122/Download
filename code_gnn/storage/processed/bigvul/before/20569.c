static enum hrtimer_restart hlt_timer_fn(struct hrtimer *data)
{
	struct kvm_vcpu *vcpu;
	wait_queue_head_t *q;

	vcpu  = container_of(data, struct kvm_vcpu, arch.hlt_timer);
	q = &vcpu->wq;

	if (vcpu->arch.mp_state != KVM_MP_STATE_HALTED)
		goto out;

	if (waitqueue_active(q))
		wake_up_interruptible(q);

out:
	vcpu->arch.timer_fired = 1;
	vcpu->arch.timer_check = 1;
	return HRTIMER_NORESTART;
}
