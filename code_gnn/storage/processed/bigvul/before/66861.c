void __init init_mem_mapping(void)
{
	unsigned long end;

	probe_page_size_mask();

#ifdef CONFIG_X86_64
	end = max_pfn << PAGE_SHIFT;
#else
	end = max_low_pfn << PAGE_SHIFT;
#endif

	 
	init_memory_mapping(0, ISA_END_ADDRESS);

	 
	init_trampoline();

	 
	if (memblock_bottom_up()) {
		unsigned long kernel_end = __pa_symbol(_end);

		 
		memory_map_bottom_up(kernel_end, end);
		memory_map_bottom_up(ISA_END_ADDRESS, kernel_end);
	} else {
		memory_map_top_down(ISA_END_ADDRESS, end);
	}

#ifdef CONFIG_X86_64
	if (max_pfn > max_low_pfn) {
		 
		max_low_pfn = max_pfn;
	}
#else
	early_ioremap_page_table_range_init();
#endif

	load_cr3(swapper_pg_dir);
	__flush_tlb_all();

	early_memtest(0, max_pfn_mapped << PAGE_SHIFT);
}
