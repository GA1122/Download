static void nested_vmx_abort(struct kvm_vcpu *vcpu, u32 indicator)
{
	 
	kvm_make_request(KVM_REQ_TRIPLE_FAULT, vcpu);
	pr_debug_ratelimited("kvm: nested vmx abort, indicator %d\n", indicator);
}
