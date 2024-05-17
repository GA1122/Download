static void kvm_patch_vmm(struct kvm_vmm_info *vmm_info,
			  struct module *module)
{
	unsigned long new_ar, new_ar_sn2;
	unsigned long module_base;

	if (!ia64_platform_is("sn2"))
		return;

	module_base = (unsigned long)module->module_core;

	new_ar = kvm_vmm_base + vmm_info->patch_mov_ar - module_base;
	new_ar_sn2 = kvm_vmm_base + vmm_info->patch_mov_ar_sn2 - module_base;

	printk(KERN_INFO "kvm: Patching ITC emulation to use SGI SN2 RTC "
	       "as source\n");

	 
	memcpy((void *)new_ar, (void *)new_ar_sn2, 0x60);
}
