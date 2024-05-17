get_total_entries(struct trace_buffer *buf,
		  unsigned long *total, unsigned long *entries)
{
	unsigned long count;
	int cpu;

	*total = 0;
	*entries = 0;

	for_each_tracing_cpu(cpu) {
		count = ring_buffer_entries_cpu(buf->buffer, cpu);
		 
		if (per_cpu_ptr(buf->data, cpu)->skipped_entries) {
			count -= per_cpu_ptr(buf->data, cpu)->skipped_entries;
			 
			*total += count;
		} else
			*total += count +
				ring_buffer_overrun_cpu(buf->buffer, cpu);
		*entries += count;
	}
}
