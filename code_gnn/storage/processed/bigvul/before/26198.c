static void sw_perf_event_destroy(struct perf_event *event)
{
	u64 event_id = event->attr.config;

	WARN_ON(event->parent);

	jump_label_dec(&perf_swevent_enabled[event_id]);
	swevent_hlist_put(event);
}
