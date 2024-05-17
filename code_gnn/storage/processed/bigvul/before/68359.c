perf_event_exit_event(struct perf_event *child_event,
		      struct perf_event_context *child_ctx,
		      struct task_struct *child)
{
	struct perf_event *parent_event = child_event->parent;

	 
	raw_spin_lock_irq(&child_ctx->lock);
	WARN_ON_ONCE(child_ctx->is_active);

	if (parent_event)
		perf_group_detach(child_event);
	list_del_event(child_event, child_ctx);
	child_event->state = PERF_EVENT_STATE_EXIT;  
	raw_spin_unlock_irq(&child_ctx->lock);

	 
	if (!parent_event) {
		perf_event_wakeup(child_event);
		return;
	}
	 

	sync_child_event(child_event, child);

	 
	WARN_ON_ONCE(parent_event->ctx->parent_ctx);
	mutex_lock(&parent_event->child_mutex);
	list_del_init(&child_event->child_list);
	mutex_unlock(&parent_event->child_mutex);

	 
	perf_event_wakeup(parent_event);
	free_event(child_event);
	put_event(parent_event);
}
