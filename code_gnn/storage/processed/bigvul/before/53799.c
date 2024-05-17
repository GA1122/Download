static void __init reserve_crashkernel(void)
{
	unsigned long long crash_size, crash_base, total_mem;
	bool high = false;
	int ret;

	total_mem = memblock_phys_mem_size();

	 
	ret = parse_crashkernel(boot_command_line, total_mem, &crash_size, &crash_base);
	if (ret != 0 || crash_size <= 0) {
		 
		ret = parse_crashkernel_high(boot_command_line, total_mem,
					     &crash_size, &crash_base);
		if (ret != 0 || crash_size <= 0)
			return;
		high = true;
	}

	 
	if (crash_base <= 0) {
		 
		crash_base = memblock_find_in_range(CRASH_ALIGN,
						    high ? CRASH_ADDR_HIGH_MAX
							 : CRASH_ADDR_LOW_MAX,
						    crash_size, CRASH_ALIGN);
		if (!crash_base) {
			pr_info("crashkernel reservation failed - No suitable area found.\n");
			return;
		}

	} else {
		unsigned long long start;

		start = memblock_find_in_range(crash_base,
					       crash_base + crash_size,
					       crash_size, 1 << 20);
		if (start != crash_base) {
			pr_info("crashkernel reservation failed - memory is in use.\n");
			return;
		}
	}
	ret = memblock_reserve(crash_base, crash_size);
	if (ret) {
		pr_err("%s: Error reserving crashkernel memblock.\n", __func__);
		return;
	}

	if (crash_base >= (1ULL << 32) && reserve_crashkernel_low()) {
		memblock_free(crash_base, crash_size);
		return;
	}

	pr_info("Reserving %ldMB of memory at %ldMB for crashkernel (System RAM: %ldMB)\n",
		(unsigned long)(crash_size >> 20),
		(unsigned long)(crash_base >> 20),
		(unsigned long)(total_mem >> 20));

	crashk_res.start = crash_base;
	crashk_res.end   = crash_base + crash_size - 1;
	insert_resource(&iomem_resource, &crashk_res);
}
