static int vti_vcpu_setup(struct kvm_vcpu *vcpu, int id)
{
	unsigned long psr;
	int r;

	local_irq_save(psr);
	r = kvm_insert_vmm_mapping(vcpu);
	local_irq_restore(psr);
	if (r)
		goto fail;
	r = kvm_vcpu_init(vcpu, vcpu->kvm, id);
	if (r)
		goto fail;

	r = vti_init_vpd(vcpu);
	if (r) {
		printk(KERN_DEBUG"kvm: vpd init error!!\n");
		goto uninit;
	}

	r = vti_create_vp(vcpu);
	if (r)
		goto uninit;

	kvm_purge_vmm_mapping(vcpu);

	return 0;
uninit:
	kvm_vcpu_uninit(vcpu);
fail:
	return r;
}
