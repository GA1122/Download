static int kvm_relocate_vmm(struct kvm_vmm_info *vmm_info,
			    struct module *module)
{
	unsigned long module_base;
	unsigned long vmm_size;

	unsigned long vmm_offset, func_offset, fdesc_offset;
	struct fdesc *p_fdesc;

	BUG_ON(!module);

	if (!kvm_vmm_base) {
		printk("kvm: kvm area hasn't been initialized yet!!\n");
		return -EFAULT;
	}

	 
	module_base = (unsigned long)module->module_core;
	vmm_size = module->core_size;
	if (unlikely(vmm_size > KVM_VMM_SIZE))
		return -EFAULT;

	memcpy((void *)kvm_vmm_base, (void *)module_base, vmm_size);
	kvm_patch_vmm(vmm_info, module);
	kvm_flush_icache(kvm_vmm_base, vmm_size);

	 
	vmm_offset = vmm_info->vmm_ivt - module_base;
	kvm_vmm_info->vmm_ivt = KVM_VMM_BASE + vmm_offset;
	printk(KERN_DEBUG"kvm: Relocated VMM's IVT Base Addr:%lx\n",
			kvm_vmm_info->vmm_ivt);

	fdesc_offset = (unsigned long)vmm_info->vmm_entry - module_base;
	kvm_vmm_info->vmm_entry = (kvm_vmm_entry *)(KVM_VMM_BASE +
							fdesc_offset);
	func_offset = *(unsigned long *)vmm_info->vmm_entry - module_base;
	p_fdesc = (struct fdesc *)(kvm_vmm_base + fdesc_offset);
	p_fdesc->ip = KVM_VMM_BASE + func_offset;
	p_fdesc->gp = KVM_VMM_BASE+(p_fdesc->gp - module_base);

	printk(KERN_DEBUG"kvm: Relocated VMM's Init Entry Addr:%lx\n",
			KVM_VMM_BASE+func_offset);

	fdesc_offset = (unsigned long)vmm_info->tramp_entry - module_base;
	kvm_vmm_info->tramp_entry = (kvm_tramp_entry *)(KVM_VMM_BASE +
			fdesc_offset);
	func_offset = *(unsigned long *)vmm_info->tramp_entry - module_base;
	p_fdesc = (struct fdesc *)(kvm_vmm_base + fdesc_offset);
	p_fdesc->ip = KVM_VMM_BASE + func_offset;
	p_fdesc->gp = KVM_VMM_BASE + (p_fdesc->gp - module_base);

	kvm_vmm_gp = p_fdesc->gp;

	printk(KERN_DEBUG"kvm: Relocated VMM's Entry IP:%p\n",
						kvm_vmm_info->vmm_entry);
	printk(KERN_DEBUG"kvm: Relocated VMM's Trampoline Entry IP:0x%lx\n",
						KVM_VMM_BASE + func_offset);

	return 0;
}
