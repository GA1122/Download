static void __init probe_page_size_mask(void)
{
#if !defined(CONFIG_KMEMCHECK)
	 
	if (boot_cpu_has(X86_FEATURE_PSE) && !debug_pagealloc_enabled())
		page_size_mask |= 1 << PG_LEVEL_2M;
#endif

	 
	if (boot_cpu_has(X86_FEATURE_PSE))
		cr4_set_bits_and_update_boot(X86_CR4_PSE);

	 
	if (boot_cpu_has(X86_FEATURE_PGE)) {
		cr4_set_bits_and_update_boot(X86_CR4_PGE);
		__supported_pte_mask |= _PAGE_GLOBAL;
	} else
		__supported_pte_mask &= ~_PAGE_GLOBAL;

	 
	if (direct_gbpages && boot_cpu_has(X86_FEATURE_GBPAGES)) {
		printk(KERN_INFO "Using GB pages for direct mapping\n");
		page_size_mask |= 1 << PG_LEVEL_1G;
	} else {
		direct_gbpages = 0;
	}
}
