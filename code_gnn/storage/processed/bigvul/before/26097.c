static u64 perf_event_read(struct perf_event *event)
{
	 
	if (event->state == PERF_EVENT_STATE_ACTIVE) {
		smp_call_function_single(event->oncpu,
					 __perf_event_read, event, 1);
	} else if (event->state == PERF_EVENT_STATE_INACTIVE) {
		struct perf_event_context *ctx = event->ctx;
		unsigned long flags;

		raw_spin_lock_irqsave(&ctx->lock, flags);
		 
		if (ctx->is_active) {
			update_context_time(ctx);
			update_cgrp_time_from_event(event);
		}
		update_event_times(event);
		raw_spin_unlock_irqrestore(&ctx->lock, flags);
	}

	return perf_event_count(event);
}