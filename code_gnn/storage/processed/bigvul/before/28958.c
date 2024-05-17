int kvm_arch_vcpu_init(struct kvm_vcpu *vcpu)
{
	int ret;

	 
	vcpu->arch.target = -1;

	 
	ret = kvm_vgic_vcpu_init(vcpu);
	if (ret)
		return ret;

	 
	kvm_timer_vcpu_init(vcpu);

	return 0;
}
