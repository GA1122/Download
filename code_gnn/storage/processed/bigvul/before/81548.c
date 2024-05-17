static int wait_on_pipe(struct trace_iterator *iter, bool full)
{
	 
	if (trace_buffer_iter(iter, iter->cpu_file))
		return 0;

	return ring_buffer_wait(iter->trace_buffer->buffer, iter->cpu_file,
				full);
}
