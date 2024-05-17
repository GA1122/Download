static void perf_remove_from_context(struct perf_event *event)
{
	struct perf_event_context *ctx = event->ctx;
	struct task_struct *task = ctx->task;

	lockdep_assert_held(&ctx->mutex);

	if (!task) {
		 
		cpu_function_call(event->cpu, __perf_remove_from_context, event);
		return;
	}

retry:
	if (!task_function_call(task, __perf_remove_from_context, event))
		return;

	raw_spin_lock_irq(&ctx->lock);
	 
	if (ctx->is_active) {
		raw_spin_unlock_irq(&ctx->lock);
		goto retry;
	}

	 
	list_del_event(event, ctx);
	raw_spin_unlock_irq(&ctx->lock);
}
