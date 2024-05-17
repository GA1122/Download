static void __init memory_map_bottom_up(unsigned long map_start,
					unsigned long map_end)
{
	unsigned long next, start;
	unsigned long mapped_ram_size = 0;
	 
	unsigned long step_size = PMD_SIZE;

	start = map_start;
	min_pfn_mapped = start >> PAGE_SHIFT;

	 
	while (start < map_end) {
		if (step_size && map_end - start > step_size) {
			next = round_up(start + 1, step_size);
			if (next > map_end)
				next = map_end;
		} else {
			next = map_end;
		}

		mapped_ram_size += init_range_memory_mapping(start, next);
		start = next;

		if (mapped_ram_size >= step_size)
			step_size = get_new_step_size(step_size);
	}
}
