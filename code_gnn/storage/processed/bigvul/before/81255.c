static u64 buffer_ftrace_now(struct trace_buffer *buf, int cpu)
{
	u64 ts;

	 
	if (!buf->buffer)
		return trace_clock_local();

	ts = ring_buffer_time_stamp(buf->buffer, cpu);
	ring_buffer_normalize_time_stamp(buf->buffer, cpu, &ts);

	return ts;
}
