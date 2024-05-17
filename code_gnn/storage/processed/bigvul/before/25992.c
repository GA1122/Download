ctx_sched_in(struct perf_event_context *ctx,
	     struct perf_cpu_context *cpuctx,
	     enum event_type_t event_type,
	     struct task_struct *task)
{
	u64 now;
	int is_active = ctx->is_active;

	ctx->is_active |= event_type;
	if (likely(!ctx->nr_events))
		return;

	now = perf_clock();
	ctx->timestamp = now;
	perf_cgroup_set_timestamp(task, ctx);
	 
	if (!(is_active & EVENT_PINNED) && (event_type & EVENT_PINNED))
		ctx_pinned_sched_in(ctx, cpuctx);

	 
	if (!(is_active & EVENT_FLEXIBLE) && (event_type & EVENT_FLEXIBLE))
		ctx_flexible_sched_in(ctx, cpuctx);
}
