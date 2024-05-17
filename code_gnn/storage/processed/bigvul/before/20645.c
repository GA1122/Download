static int vti_cpu_has_kvm_support(void)
{
	long  avail = 1, status = 1, control = 1;
	long ret;

	ret = ia64_pal_proc_get_features(&avail, &status, &control, 0);
	if (ret)
		goto out;

	if (!(avail & PAL_PROC_VM_BIT))
		goto out;

	printk(KERN_DEBUG"kvm: Hardware Supports VT\n");

	ret = ia64_pal_vp_env_info(&kvm_vm_buffer_size, &vp_env_info);
	if (ret)
		goto out;
	printk(KERN_DEBUG"kvm: VM Buffer Size:0x%lx\n", kvm_vm_buffer_size);

	if (!(vp_env_info & VP_OPCODE)) {
		printk(KERN_WARNING"kvm: No opcode ability on hardware, "
				"vm_env_info:0x%lx\n", vp_env_info);
	}

	return 1;
out:
	return 0;
}
