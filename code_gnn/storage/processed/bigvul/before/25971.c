static int  __perf_install_in_context(void *info)
{
	struct perf_event *event = info;
	struct perf_event_context *ctx = event->ctx;
	struct perf_cpu_context *cpuctx = __get_cpu_context(ctx);
	struct perf_event_context *task_ctx = cpuctx->task_ctx;
	struct task_struct *task = current;

	perf_ctx_lock(cpuctx, task_ctx);
	perf_pmu_disable(cpuctx->ctx.pmu);

	 
	if (task_ctx)
		task_ctx_sched_out(task_ctx);

	 
	if (ctx->task && task_ctx != ctx) {
		if (task_ctx)
			raw_spin_unlock(&task_ctx->lock);
		raw_spin_lock(&ctx->lock);
		task_ctx = ctx;
	}

	if (task_ctx) {
		cpuctx->task_ctx = task_ctx;
		task = task_ctx->task;
	}

	cpu_ctx_sched_out(cpuctx, EVENT_ALL);

	update_context_time(ctx);
	 
	update_cgrp_time_from_event(event);

	add_event_to_ctx(event, ctx);

	 
	perf_event_sched_in(cpuctx, task_ctx, task);

	perf_pmu_enable(cpuctx->ctx.pmu);
	perf_ctx_unlock(cpuctx, task_ctx);

	return 0;
}
