static inline void rb_event_discard(struct ring_buffer_event *event)
{
	if (event->type_len == RINGBUF_TYPE_TIME_EXTEND)
		event = skip_time_extend(event);

	 
	event->array[0] = rb_event_data_length(event) - RB_EVNT_HDR_SIZE;
	event->type_len = RINGBUF_TYPE_PADDING;
	 
	if (!event->time_delta)
		event->time_delta = 1;
}
