static void put_trace_buf(void)
{
	 
	barrier();
	this_cpu_dec(trace_percpu_buffer->nesting);
}
