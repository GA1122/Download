void kvm_arch_vcpu_destroy(struct kvm_vcpu *vcpu)
{
	kvm_vcpu_uninit(vcpu);
}