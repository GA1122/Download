static int alloc_pebs_buffer(int cpu)
{
	struct debug_store *ds = per_cpu(cpu_hw_events, cpu).ds;
	int node = cpu_to_node(cpu);
	int max, thresh = 1;  
	void *buffer;

	if (!x86_pmu.pebs)
		return 0;

	buffer = kmalloc_node(PEBS_BUFFER_SIZE, GFP_KERNEL | __GFP_ZERO, node);
	if (unlikely(!buffer))
		return -ENOMEM;

	max = PEBS_BUFFER_SIZE / x86_pmu.pebs_record_size;

	ds->pebs_buffer_base = (u64)(unsigned long)buffer;
	ds->pebs_index = ds->pebs_buffer_base;
	ds->pebs_absolute_maximum = ds->pebs_buffer_base +
		max * x86_pmu.pebs_record_size;

	ds->pebs_interrupt_threshold = ds->pebs_buffer_base +
		thresh * x86_pmu.pebs_record_size;

	return 0;
}