 void free_init_pages(char *what, unsigned long begin, unsigned long end)
{
	unsigned long begin_aligned, end_aligned;

	 
	begin_aligned = PAGE_ALIGN(begin);
	end_aligned   = end & PAGE_MASK;

	if (WARN_ON(begin_aligned != begin || end_aligned != end)) {
		begin = begin_aligned;
		end   = end_aligned;
	}

	if (begin >= end)
		return;

	 
	if (debug_pagealloc_enabled()) {
		pr_info("debug: unmapping init [mem %#010lx-%#010lx]\n",
			begin, end - 1);
		set_memory_np(begin, (end - begin) >> PAGE_SHIFT);
	} else {
		 
		set_memory_nx(begin, (end - begin) >> PAGE_SHIFT);
		set_memory_rw(begin, (end - begin) >> PAGE_SHIFT);

		free_reserved_area((void *)begin, (void *)end,
				   POISON_FREE_INITMEM, what);
	}
}
