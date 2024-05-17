static void calc_timer_values(struct perf_event *event,
				u64 *running,
				u64 *enabled)
{
	u64 now, ctx_time;

	now = perf_clock();
	ctx_time = event->shadow_ctx_time + now;
	*enabled = ctx_time - event->tstamp_enabled;
	*running = ctx_time - event->tstamp_running;
}
