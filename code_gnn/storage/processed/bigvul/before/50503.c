perf_install_in_context(struct perf_event_context *ctx,
			struct perf_event *event,
			int cpu)
{
	struct task_struct *task = ctx->task;

	lockdep_assert_held(&ctx->mutex);

	event->ctx = ctx;
	if (event->cpu != -1)
		event->cpu = cpu;

	if (!task) {
		 
		cpu_function_call(cpu, __perf_install_in_context, event);
		return;
	}

retry:
	if (!task_function_call(task, __perf_install_in_context, event))
		return;

	raw_spin_lock_irq(&ctx->lock);
	 
	if (ctx->is_active) {
		raw_spin_unlock_irq(&ctx->lock);
		 
		task = ctx->task;
		goto retry;
	}

	 
	add_event_to_ctx(event, ctx);
	raw_spin_unlock_irq(&ctx->lock);
}