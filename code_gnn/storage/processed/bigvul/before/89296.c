int arch_memory_test_prepare(u32 *vstart, u32 *size, phys_addr_t *phys_offset)
{
	*vstart = CONFIG_SYS_SDRAM_BASE;

	 
	*size = CONFIG_SYS_TEXT_BASE - (1<<20);

	return 0;
}
