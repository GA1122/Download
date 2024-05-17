static void kvm_vcpu_post_transition(struct kvm_vcpu *vcpu)
{
	kvm_purge_vmm_mapping(vcpu);
	vti_set_rr6(vcpu->arch.host_rr6);
}
