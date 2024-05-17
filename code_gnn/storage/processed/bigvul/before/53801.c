static int __init reserve_crashkernel_low(void)
{
#ifdef CONFIG_X86_64
	unsigned long long base, low_base = 0, low_size = 0;
	unsigned long total_low_mem;
	int ret;

	total_low_mem = memblock_mem_size(1UL << (32 - PAGE_SHIFT));

	 
	ret = parse_crashkernel_low(boot_command_line, total_low_mem, &low_size, &base);
	if (ret) {
		 
		low_size = max(swiotlb_size_or_default() + (8UL << 20), 256UL << 20);
	} else {
		 
		if (!low_size)
			return 0;
	}

	low_base = memblock_find_in_range(low_size, 1ULL << 32, low_size, CRASH_ALIGN);
	if (!low_base) {
		pr_err("Cannot reserve %ldMB crashkernel low memory, please try smaller size.\n",
		       (unsigned long)(low_size >> 20));
		return -ENOMEM;
	}

	ret = memblock_reserve(low_base, low_size);
	if (ret) {
		pr_err("%s: Error reserving crashkernel low memblock.\n", __func__);
		return ret;
	}

	pr_info("Reserving %ldMB of low memory at %ldMB for crashkernel (System low RAM: %ldMB)\n",
		(unsigned long)(low_size >> 20),
		(unsigned long)(low_base >> 20),
		(unsigned long)(total_low_mem >> 20));

	crashk_low_res.start = low_base;
	crashk_low_res.end   = low_base + low_size - 1;
	insert_resource(&iomem_resource, &crashk_low_res);
#endif
	return 0;
}