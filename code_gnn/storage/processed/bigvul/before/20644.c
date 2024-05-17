static int vcpu_reset(struct kvm_vcpu *vcpu)
{
	int r;
	long psr;
	local_irq_save(psr);
	r = kvm_insert_vmm_mapping(vcpu);
	local_irq_restore(psr);
	if (r)
		goto fail;

	vcpu->arch.launched = 0;
	kvm_arch_vcpu_uninit(vcpu);
	r = kvm_arch_vcpu_init(vcpu);
	if (r)
		goto fail;

	kvm_purge_vmm_mapping(vcpu);
	r = 0;
fail:
	return r;
}
