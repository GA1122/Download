void tracer_tracing_off(struct trace_array *tr)
{
	if (tr->trace_buffer.buffer)
		ring_buffer_record_off(tr->trace_buffer.buffer);
	 
	tr->buffer_disabled = 1;
	 
	smp_wmb();
}
