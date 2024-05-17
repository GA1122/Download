void kvm_arch_hardware_disable(void *garbage)
{

	long status;
	int slot;
	unsigned long pte;
	unsigned long saved_psr;
	unsigned long host_iva = ia64_getreg(_IA64_REG_CR_IVA);

	pte = pte_val(mk_pte_phys(__pa(kvm_vmm_base),
				PAGE_KERNEL));

	local_irq_save(saved_psr);
	slot = ia64_itr_entry(0x3, KVM_VMM_BASE, pte, KVM_VMM_SHIFT);
	local_irq_restore(saved_psr);
	if (slot < 0)
		return;

	status = ia64_pal_vp_exit_env(host_iva);
	if (status)
		printk(KERN_DEBUG"kvm: Failed to disable VT support! :%ld\n",
				status);
	ia64_ptr_entry(0x3, slot);
}
