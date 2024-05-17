ctx_flexible_sched_in(struct perf_event_context *ctx,
		      struct perf_cpu_context *cpuctx)
{
	struct perf_event *event;
	int can_add_hw = 1;

	list_for_each_entry(event, &ctx->flexible_groups, group_entry) {
		 
		if (event->state <= PERF_EVENT_STATE_OFF)
			continue;
		 
		if (!event_filter_match(event))
			continue;

		 
		if (is_cgroup_event(event))
			perf_cgroup_mark_enabled(event, ctx);

		if (group_can_go_on(event, cpuctx, can_add_hw)) {
			if (group_sched_in(event, cpuctx, ctx))
				can_add_hw = 0;
		}
	}
}