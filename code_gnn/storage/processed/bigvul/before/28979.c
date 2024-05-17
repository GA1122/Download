static void vcpu_pause(struct kvm_vcpu *vcpu)
{
	wait_queue_head_t *wq = kvm_arch_vcpu_wq(vcpu);

 	wait_event_interruptible(*wq, !vcpu->arch.pause);
 }