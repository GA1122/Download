tracing_free_buffer_release(struct inode *inode, struct file *filp)
{
	struct trace_array *tr = inode->i_private;

	 
	if (tr->trace_flags & TRACE_ITER_STOP_ON_FREE)
		tracer_tracing_off(tr);
	 
	tracing_resize_ring_buffer(tr, 0, RING_BUFFER_ALL_CPUS);

	trace_array_put(tr);

	return 0;
}