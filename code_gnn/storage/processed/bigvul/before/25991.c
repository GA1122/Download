ctx_pinned_sched_in(struct perf_event_context *ctx,
		    struct perf_cpu_context *cpuctx)
{
	struct perf_event *event;

	list_for_each_entry(event, &ctx->pinned_groups, group_entry) {
		if (event->state <= PERF_EVENT_STATE_OFF)
			continue;
		if (!event_filter_match(event))
			continue;

		 
		if (is_cgroup_event(event))
			perf_cgroup_mark_enabled(event, ctx);

		if (group_can_go_on(event, cpuctx, 1))
			group_sched_in(event, cpuctx, ctx);

		 
		if (event->state == PERF_EVENT_STATE_INACTIVE) {
			update_group_times(event);
			event->state = PERF_EVENT_STATE_ERROR;
		}
	}
}