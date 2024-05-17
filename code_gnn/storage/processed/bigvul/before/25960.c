static int __perf_event_disable(void *info)
{
	struct perf_event *event = info;
	struct perf_event_context *ctx = event->ctx;
	struct perf_cpu_context *cpuctx = __get_cpu_context(ctx);

	 
	if (ctx->task && cpuctx->task_ctx != ctx)
		return -EINVAL;

	raw_spin_lock(&ctx->lock);

	 
	if (event->state >= PERF_EVENT_STATE_INACTIVE) {
		update_context_time(ctx);
		update_cgrp_time_from_event(event);
		update_group_times(event);
		if (event == event->group_leader)
			group_sched_out(event, cpuctx, ctx);
		else
			event_sched_out(event, cpuctx, ctx);
		event->state = PERF_EVENT_STATE_OFF;
	}

	raw_spin_unlock(&ctx->lock);

	return 0;
}
