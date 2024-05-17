trace_poll(struct trace_iterator *iter, struct file *filp, poll_table *poll_table)
{
	struct trace_array *tr = iter->tr;

	 
	if (trace_buffer_iter(iter, iter->cpu_file))
		return EPOLLIN | EPOLLRDNORM;

	if (tr->trace_flags & TRACE_ITER_BLOCK)
		 
		return EPOLLIN | EPOLLRDNORM;
	else
		return ring_buffer_poll_wait(iter->trace_buffer->buffer, iter->cpu_file,
					     filp, poll_table);
}
