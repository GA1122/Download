void kvm_set_pending_timer(struct kvm_vcpu *vcpu)
{
	 
	kvm_make_request(KVM_REQ_PENDING_TIMER, vcpu);
}
