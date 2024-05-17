__buffer_unlock_commit(struct ring_buffer *buffer, struct ring_buffer_event *event)
{
	__this_cpu_write(trace_taskinfo_save, true);

	 
	if (this_cpu_read(trace_buffered_event) == event) {
		 
		ring_buffer_write(buffer, event->array[0], &event->array[1]);
		 
		this_cpu_dec(trace_buffered_event_cnt);
	} else
		ring_buffer_unlock_commit(buffer, event);
}
