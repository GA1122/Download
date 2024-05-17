static void perf_remove_from_context(struct perf_event *event, unsigned long flags)
{
	struct perf_event_context *ctx = event->ctx;

	lockdep_assert_held(&ctx->mutex);

	event_function_call(event, __perf_remove_from_context, (void *)flags);

	 
	WARN_ON_ONCE(event->attach_state & PERF_ATTACH_CONTEXT);
	if ((flags & DETACH_GROUP) &&
	    (event->attach_state & PERF_ATTACH_GROUP)) {
		 
		raw_spin_lock_irq(&ctx->lock);
		perf_group_detach(event);
		raw_spin_unlock_irq(&ctx->lock);
	}
}
