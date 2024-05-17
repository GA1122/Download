static void perf_event_exit_task_context(struct task_struct *child, int ctxn)
{
	struct perf_event *child_event, *tmp;
	struct perf_event_context *child_ctx;
	unsigned long flags;

	if (likely(!child->perf_event_ctxp[ctxn])) {
		perf_event_task(child, NULL, 0);
		return;
	}

	local_irq_save(flags);
	 
	child_ctx = rcu_dereference_raw(child->perf_event_ctxp[ctxn]);

	 
	raw_spin_lock(&child_ctx->lock);
	task_ctx_sched_out(child_ctx);
	child->perf_event_ctxp[ctxn] = NULL;
	 
	unclone_ctx(child_ctx);
	update_context_time(child_ctx);
	raw_spin_unlock_irqrestore(&child_ctx->lock, flags);

	 
	perf_event_task(child, child_ctx, 0);

	 
	mutex_lock(&child_ctx->mutex);

again:
	list_for_each_entry_safe(child_event, tmp, &child_ctx->pinned_groups,
				 group_entry)
		__perf_event_exit_task(child_event, child_ctx, child);

	list_for_each_entry_safe(child_event, tmp, &child_ctx->flexible_groups,
				 group_entry)
		__perf_event_exit_task(child_event, child_ctx, child);

	 
	if (!list_empty(&child_ctx->pinned_groups) ||
	    !list_empty(&child_ctx->flexible_groups))
		goto again;

	mutex_unlock(&child_ctx->mutex);

	put_ctx(child_ctx);
}
