static inline struct fasync_struct **perf_event_fasync(struct perf_event *event)
{
	 
	if (event->parent)
		event = event->parent;
	return &event->fasync;
}
