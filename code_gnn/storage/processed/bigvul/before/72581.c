static inline u64 rb_time_stamp(struct ring_buffer *buffer)
{
	 
	return buffer->clock() << DEBUG_SHIFT;
}
