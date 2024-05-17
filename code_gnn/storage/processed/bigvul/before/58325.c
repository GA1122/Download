static void __soft_restart(void *addr)
{
	phys_reset_t phys_reset;

	 
	setup_mm_for_reboot();

	 
	flush_cache_all();

	 
	cpu_proc_fin();

	 
	flush_cache_all();

	 
	phys_reset = (phys_reset_t)(unsigned long)virt_to_phys(cpu_reset);
	phys_reset((unsigned long)addr);

	 
	BUG();
}
