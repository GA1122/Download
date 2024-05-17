int kvm_arch_hardware_enable(void *garbage)
{
	long  status;
	long  tmp_base;
	unsigned long pte;
	unsigned long saved_psr;
	int slot;

	pte = pte_val(mk_pte_phys(__pa(kvm_vmm_base), PAGE_KERNEL));
	local_irq_save(saved_psr);
	slot = ia64_itr_entry(0x3, KVM_VMM_BASE, pte, KVM_VMM_SHIFT);
	local_irq_restore(saved_psr);
	if (slot < 0)
		return -EINVAL;

	spin_lock(&vp_lock);
	status = ia64_pal_vp_init_env(kvm_vsa_base ?
				VP_INIT_ENV : VP_INIT_ENV_INITALIZE,
			__pa(kvm_vm_buffer), KVM_VM_BUFFER_BASE, &tmp_base);
	if (status != 0) {
		spin_unlock(&vp_lock);
		printk(KERN_WARNING"kvm: Failed to Enable VT Support!!!!\n");
		return -EINVAL;
	}

	if (!kvm_vsa_base) {
		kvm_vsa_base = tmp_base;
		printk(KERN_INFO"kvm: kvm_vsa_base:0x%lx\n", kvm_vsa_base);
	}
	spin_unlock(&vp_lock);
	ia64_ptr_entry(0x3, slot);

	return 0;
}
