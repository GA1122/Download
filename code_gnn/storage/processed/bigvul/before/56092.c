static int perf_event_read(struct perf_event *event, bool group)
{
	int ret = 0;

	 
	if (event->state == PERF_EVENT_STATE_ACTIVE) {
		struct perf_read_data data = {
			.event = event,
			.group = group,
			.ret = 0,
		};
		smp_call_function_single(event->oncpu,
					 __perf_event_read, &data, 1);
		ret = data.ret;
	} else if (event->state == PERF_EVENT_STATE_INACTIVE) {
		struct perf_event_context *ctx = event->ctx;
		unsigned long flags;

		raw_spin_lock_irqsave(&ctx->lock, flags);
		 
		if (ctx->is_active) {
			update_context_time(ctx);
			update_cgrp_time_from_event(event);
		}
		if (group)
			update_group_times(event);
		else
			update_event_times(event);
		raw_spin_unlock_irqrestore(&ctx->lock, flags);
	}

	return ret;
}