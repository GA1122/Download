rb_event_data(struct ring_buffer_event *event)
{
	if (event->type_len == RINGBUF_TYPE_TIME_EXTEND)
		event = skip_time_extend(event);
	BUG_ON(event->type_len > RINGBUF_TYPE_DATA_TYPE_LEN_MAX);
	 
	if (event->type_len)
		return (void *)&event->array[0];
	 
	return (void *)&event->array[1];
}
