int kvm_arch_init(void *opaque)
{
	int r;
	struct kvm_vmm_info *vmm_info = (struct kvm_vmm_info *)opaque;

	if (!vti_cpu_has_kvm_support()) {
		printk(KERN_ERR "kvm: No Hardware Virtualization Support!\n");
		r = -EOPNOTSUPP;
		goto out;
	}

	if (kvm_vmm_info) {
		printk(KERN_ERR "kvm: Already loaded VMM module!\n");
		r = -EEXIST;
		goto out;
	}

	r = -ENOMEM;
	kvm_vmm_info = kzalloc(sizeof(struct kvm_vmm_info), GFP_KERNEL);
	if (!kvm_vmm_info)
		goto out;

	if (kvm_alloc_vmm_area())
		goto out_free0;

	r = kvm_relocate_vmm(vmm_info, vmm_info->module);
	if (r)
		goto out_free1;

	return 0;

out_free1:
	kvm_free_vmm_area();
out_free0:
	kfree(kvm_vmm_info);
out:
	return r;
}
