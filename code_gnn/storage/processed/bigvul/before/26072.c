static void perf_event_enable_on_exec(struct perf_event_context *ctx)
{
	struct perf_event *event;
	unsigned long flags;
	int enabled = 0;
	int ret;

	local_irq_save(flags);
	if (!ctx || !ctx->nr_events)
		goto out;

	 
	perf_cgroup_sched_out(current);

	raw_spin_lock(&ctx->lock);
	task_ctx_sched_out(ctx);

	list_for_each_entry(event, &ctx->pinned_groups, group_entry) {
		ret = event_enable_on_exec(event, ctx);
		if (ret)
			enabled = 1;
	}

	list_for_each_entry(event, &ctx->flexible_groups, group_entry) {
		ret = event_enable_on_exec(event, ctx);
		if (ret)
			enabled = 1;
	}

	 
	if (enabled)
		unclone_ctx(ctx);

	raw_spin_unlock(&ctx->lock);

	 
	perf_event_context_sched_in(ctx, ctx->task);
out:
	local_irq_restore(flags);
}
