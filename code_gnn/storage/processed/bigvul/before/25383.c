static void save_raw_perf_callchain(struct perf_callchain_entry *entry,
	unsigned long reg29)
{
	unsigned long *sp = (unsigned long *)reg29;
	unsigned long addr;

	while (!kstack_end(sp)) {
		addr = *sp++;
		if (__kernel_text_address(addr)) {
			perf_callchain_store(entry, addr);
			if (entry->nr >= PERF_MAX_STACK_DEPTH)
				break;
		}
	}
}
