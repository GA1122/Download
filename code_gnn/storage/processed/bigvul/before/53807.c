static void __init trim_bios_range(void)
{
	 
	e820_update_range(0, PAGE_SIZE, E820_RAM, E820_RESERVED);

	 
	e820_remove_range(BIOS_BEGIN, BIOS_END - BIOS_BEGIN, E820_RAM, 1);

	sanitize_e820_map(e820.map, ARRAY_SIZE(e820.map), &e820.nr_map);
}
