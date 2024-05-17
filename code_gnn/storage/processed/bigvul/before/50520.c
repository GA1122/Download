 static void perf_set_shadow_time(struct perf_event *event,
				 struct perf_event_context *ctx,
				 u64 tstamp)
{
	 
	if (is_cgroup_event(event))
		perf_cgroup_set_shadow_time(event, tstamp);
	else
		event->shadow_ctx_time = tstamp - ctx->timestamp;
}
