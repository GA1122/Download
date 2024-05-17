list_del_event(struct perf_event *event, struct perf_event_context *ctx)
{
	struct perf_cpu_context *cpuctx;

	WARN_ON_ONCE(event->ctx != ctx);
	lockdep_assert_held(&ctx->lock);

	 
	if (!(event->attach_state & PERF_ATTACH_CONTEXT))
		return;

	event->attach_state &= ~PERF_ATTACH_CONTEXT;

	if (is_cgroup_event(event)) {
		ctx->nr_cgroups--;
		cpuctx = __get_cpu_context(ctx);
		 
		if (!ctx->nr_cgroups)
			cpuctx->cgrp = NULL;
	}

	if (has_branch_stack(event))
		ctx->nr_branch_stack--;

	ctx->nr_events--;
	if (event->attr.inherit_stat)
		ctx->nr_stat--;

	list_del_rcu(&event->event_entry);

	if (event->group_leader == event)
		list_del_init(&event->group_entry);

	update_group_times(event);

	 
	if (event->state > PERF_EVENT_STATE_OFF)
		event->state = PERF_EVENT_STATE_OFF;

	ctx->generation++;
}
