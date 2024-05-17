static int vti_create_vp(struct kvm_vcpu *vcpu)
{
	long ret;
	struct vpd *vpd = vcpu->arch.vpd;
	unsigned long  vmm_ivt;

	vmm_ivt = kvm_vmm_info->vmm_ivt;

	printk(KERN_DEBUG "kvm: vcpu:%p,ivt: 0x%lx\n", vcpu, vmm_ivt);

	ret = ia64_pal_vp_create((u64 *)vpd, (u64 *)vmm_ivt, 0);

	if (ret) {
		printk(KERN_ERR"kvm: ia64_pal_vp_create failed!\n");
		return -EINVAL;
	}
	return 0;
}
