void tracer_tracing_on(struct trace_array *tr)
{
	if (tr->trace_buffer.buffer)
		ring_buffer_record_on(tr->trace_buffer.buffer);
	 
	tr->buffer_disabled = 0;
	 
	smp_wmb();
}
