static void rb_event_set_padding(struct ring_buffer_event *event)
{
	 
	event->type_len = RINGBUF_TYPE_PADDING;
	event->time_delta = 0;
}
