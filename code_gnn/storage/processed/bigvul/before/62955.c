static void ept_set_mmio_spte_mask(void)
{
	 
	kvm_mmu_set_mmio_spte_mask(VMX_EPT_RWX_MASK,
				   VMX_EPT_MISCONFIG_WX_VALUE);
}
