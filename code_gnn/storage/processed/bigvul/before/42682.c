static void kvm_flush_pml_buffers(struct kvm *kvm)
{
	int i;
	struct kvm_vcpu *vcpu;
	 
	kvm_for_each_vcpu(i, vcpu, kvm)
		kvm_vcpu_kick(vcpu);
}
