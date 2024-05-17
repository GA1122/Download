static void perf_event_exit_task_context(struct task_struct *child, int ctxn)
{
	struct perf_event_context *child_ctx, *clone_ctx = NULL;
	struct perf_event *child_event, *next;

	WARN_ON_ONCE(child != current);

	child_ctx = perf_pin_task_context(child, ctxn);
	if (!child_ctx)
		return;

	 
	mutex_lock(&child_ctx->mutex);

	 
	raw_spin_lock_irq(&child_ctx->lock);
	task_ctx_sched_out(__get_cpu_context(child_ctx), child_ctx, EVENT_ALL);

	 
	RCU_INIT_POINTER(child->perf_event_ctxp[ctxn], NULL);
	put_ctx(child_ctx);  
	WRITE_ONCE(child_ctx->task, TASK_TOMBSTONE);
	put_task_struct(current);  

	clone_ctx = unclone_ctx(child_ctx);
	raw_spin_unlock_irq(&child_ctx->lock);

	if (clone_ctx)
		put_ctx(clone_ctx);

	 
	perf_event_task(child, child_ctx, 0);

	list_for_each_entry_safe(child_event, next, &child_ctx->event_list, event_entry)
		perf_event_exit_event(child_event, child_ctx, child);

	mutex_unlock(&child_ctx->mutex);

	put_ctx(child_ctx);
}
