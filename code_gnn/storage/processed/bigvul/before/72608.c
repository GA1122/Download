void ring_buffer_normalize_time_stamp(struct ring_buffer *buffer,
				      int cpu, u64 *ts)
{
	 
	*ts >>= DEBUG_SHIFT;
}
