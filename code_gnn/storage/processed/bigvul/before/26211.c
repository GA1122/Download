static void task_clock_event_start(struct perf_event *event, int flags)
{
	local64_set(&event->hw.prev_count, event->ctx->time);
	perf_swevent_start_hrtimer(event);
}