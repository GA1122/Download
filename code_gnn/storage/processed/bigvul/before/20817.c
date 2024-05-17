static void kvm_set_mmio_spte_mask(void)
{
	u64 mask;
	int maxphyaddr = boot_cpu_data.x86_phys_bits;

	 
	mask = ((1ull << (62 - maxphyaddr + 1)) - 1) << maxphyaddr;
	mask |= 1ull;

#ifdef CONFIG_X86_64
	 
	if (maxphyaddr == 52)
		mask &= ~1ull;
#endif

	kvm_mmu_set_mmio_spte_mask(mask);
}
