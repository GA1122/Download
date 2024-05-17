unsigned long ring_buffer_size(struct ring_buffer *buffer, int cpu)
{
	 
	if (!cpumask_test_cpu(cpu, buffer->cpumask))
		return 0;

	return BUF_PAGE_SIZE * buffer->buffers[cpu]->nr_pages;
}
