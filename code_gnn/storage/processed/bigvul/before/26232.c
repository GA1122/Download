void perf_output_end(struct perf_output_handle *handle)
{
	struct perf_event *event = handle->event;
	struct ring_buffer *rb = handle->rb;

	if (handle->sample && !event->attr.watermark) {
		int wakeup_events = event->attr.wakeup_events;

		if (wakeup_events) {
			int events = local_inc_return(&rb->events);
			if (events >= wakeup_events) {
				local_sub(wakeup_events, &rb->events);
				local_inc(&rb->wakeup);
			}
		}
	}

	perf_output_put_handle(handle);
	rcu_read_unlock();
}
