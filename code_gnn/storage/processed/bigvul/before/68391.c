perf_install_in_context(struct perf_event_context *ctx,
			struct perf_event *event,
			int cpu)
{
	struct task_struct *task = READ_ONCE(ctx->task);

	lockdep_assert_held(&ctx->mutex);

	if (event->cpu != -1)
		event->cpu = cpu;

	 
	smp_store_release(&event->ctx, ctx);

	if (!task) {
		cpu_function_call(cpu, __perf_install_in_context, event);
		return;
	}

	 
	if (WARN_ON_ONCE(task == TASK_TOMBSTONE))
		return;

	 

	 
	smp_mb();
again:
	if (!task_function_call(task, __perf_install_in_context, event))
		return;

	raw_spin_lock_irq(&ctx->lock);
	task = ctx->task;
	if (WARN_ON_ONCE(task == TASK_TOMBSTONE)) {
		 
		raw_spin_unlock_irq(&ctx->lock);
		return;
	}
	 
	if (task_curr(task)) {
		raw_spin_unlock_irq(&ctx->lock);
		goto again;
	}
	add_event_to_ctx(event, ctx);
	raw_spin_unlock_irq(&ctx->lock);
}
