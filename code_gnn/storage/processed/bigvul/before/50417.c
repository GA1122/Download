static int __perf_event_enable(void *info)
{
	struct perf_event *event = info;
	struct perf_event_context *ctx = event->ctx;
	struct perf_event *leader = event->group_leader;
	struct perf_cpu_context *cpuctx = __get_cpu_context(ctx);
	int err;

	 
	if (!ctx->is_active)
		return -EINVAL;

	raw_spin_lock(&ctx->lock);
	update_context_time(ctx);

	if (event->state >= PERF_EVENT_STATE_INACTIVE)
		goto unlock;

	 
	perf_cgroup_set_timestamp(current, ctx);

	__perf_event_mark_enabled(event);

	if (!event_filter_match(event)) {
		if (is_cgroup_event(event))
			perf_cgroup_defer_enabled(event);
		goto unlock;
	}

	 
	if (leader != event && leader->state != PERF_EVENT_STATE_ACTIVE)
		goto unlock;

	if (!group_can_go_on(event, cpuctx, 1)) {
		err = -EEXIST;
	} else {
		if (event == leader)
			err = group_sched_in(event, cpuctx, ctx);
		else
			err = event_sched_in(event, cpuctx, ctx);
	}

	if (err) {
		 
		if (leader != event) {
			group_sched_out(leader, cpuctx, ctx);
			perf_cpu_hrtimer_restart(cpuctx);
		}
		if (leader->attr.pinned) {
			update_group_times(leader);
			leader->state = PERF_EVENT_STATE_ERROR;
		}
	}

unlock:
	raw_spin_unlock(&ctx->lock);

	return 0;
}
