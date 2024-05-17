static void perf_remove_from_context(struct perf_event *event, bool detach_group)
{
	struct perf_event_context *ctx = event->ctx;
	struct task_struct *task = ctx->task;
	struct remove_event re = {
		.event = event,
		.detach_group = detach_group,
	};

	lockdep_assert_held(&ctx->mutex);

	if (!task) {
		 
		cpu_function_call(event->cpu, __perf_remove_from_context, &re);
		return;
	}

retry:
	if (!task_function_call(task, __perf_remove_from_context, &re))
		return;

	raw_spin_lock_irq(&ctx->lock);
	 
	if (ctx->is_active) {
		raw_spin_unlock_irq(&ctx->lock);
		 
		task = ctx->task;
		goto retry;
	}

	 
	if (detach_group)
		perf_group_detach(event);
	list_del_event(event, ctx);
	raw_spin_unlock_irq(&ctx->lock);
}
