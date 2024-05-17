rb_add_time_stamp(struct ring_buffer_event *event, u64 delta)
{
	event->type_len = RINGBUF_TYPE_TIME_EXTEND;

	 
	if (rb_event_index(event)) {
		event->time_delta = delta & TS_MASK;
		event->array[0] = delta >> TS_SHIFT;
	} else {
		 
		event->time_delta = 0;
		event->array[0] = 0;
	}

	return skip_time_extend(event);
}