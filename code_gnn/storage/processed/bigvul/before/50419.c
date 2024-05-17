__perf_event_exit_task(struct perf_event *child_event,
			 struct perf_event_context *child_ctx,
			 struct task_struct *child)
{
	 
	perf_remove_from_context(child_event, !!child_event->parent);

	 
	if (child_event->parent) {
		sync_child_event(child_event, child);
		free_event(child_event);
	} else {
		child_event->state = PERF_EVENT_STATE_EXIT;
		perf_event_wakeup(child_event);
	}
}
