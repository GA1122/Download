static int __perf_remove_from_context(void *info)
{
	struct perf_event *event = info;
	struct perf_event_context *ctx = event->ctx;
	struct perf_cpu_context *cpuctx = __get_cpu_context(ctx);

	raw_spin_lock(&ctx->lock);
	event_sched_out(event, cpuctx, ctx);
	list_del_event(event, ctx);
	if (!ctx->nr_events && cpuctx->task_ctx == ctx) {
		ctx->is_active = 0;
		cpuctx->task_ctx = NULL;
	}
	raw_spin_unlock(&ctx->lock);

	return 0;
}