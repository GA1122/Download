void kvm_arch_vcpu_put(struct kvm_vcpu *vcpu)
{
	kvm_x86_ops->vcpu_put(vcpu);
	kvm_put_guest_fpu(vcpu);
	vcpu->arch.last_guest_tsc = kvm_x86_ops->read_l1_tsc(vcpu);
}