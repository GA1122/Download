group_sched_out(struct perf_event *group_event,
		struct perf_cpu_context *cpuctx,
		struct perf_event_context *ctx)
{
	struct perf_event *event;
	int state = group_event->state;

	event_sched_out(group_event, cpuctx, ctx);

	 
	list_for_each_entry(event, &group_event->sibling_list, group_entry)
		event_sched_out(event, cpuctx, ctx);

	if (state == PERF_EVENT_STATE_ACTIVE && group_event->attr.exclusive)
		cpuctx->exclusive = 0;
}