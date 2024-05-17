static void perf_ctx_adjust_freq(struct perf_event_context *ctx, u64 period)
{
	struct perf_event *event;
	struct hw_perf_event *hwc;
	u64 interrupts, now;
	s64 delta;

	list_for_each_entry_rcu(event, &ctx->event_list, event_entry) {
		if (event->state != PERF_EVENT_STATE_ACTIVE)
			continue;

		if (!event_filter_match(event))
			continue;

		hwc = &event->hw;

		interrupts = hwc->interrupts;
		hwc->interrupts = 0;

		 
		if (interrupts == MAX_INTERRUPTS) {
			perf_log_throttle(event, 1);
			event->pmu->start(event, 0);
		}

		if (!event->attr.freq || !event->attr.sample_freq)
			continue;

		event->pmu->read(event);
		now = local64_read(&event->count);
		delta = now - hwc->freq_count_stamp;
		hwc->freq_count_stamp = now;

		if (delta > 0)
			perf_adjust_period(event, period, delta);
	}
}
