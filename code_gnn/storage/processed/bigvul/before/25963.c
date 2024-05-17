__perf_event_exit_task(struct perf_event *child_event,
			 struct perf_event_context *child_ctx,
			 struct task_struct *child)
{
	if (child_event->parent) {
		raw_spin_lock_irq(&child_ctx->lock);
		perf_group_detach(child_event);
		raw_spin_unlock_irq(&child_ctx->lock);
	}

	perf_remove_from_context(child_event);

	 
	if (child_event->parent) {
		sync_child_event(child_event, child);
		free_event(child_event);
	}
}
