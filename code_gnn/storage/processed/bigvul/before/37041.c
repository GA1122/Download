static void ept_set_mmio_spte_mask(void)
{
	 
	kvm_mmu_set_mmio_spte_mask((0x3ull << 62) | 0x6ull);
}
