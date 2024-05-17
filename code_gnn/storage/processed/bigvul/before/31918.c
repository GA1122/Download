static void perf_adjust_freq_unthr_context(struct perf_event_context *ctx,
					   int needs_unthr)
{
	struct perf_event *event;
	struct hw_perf_event *hwc;
	u64 now, period = TICK_NSEC;
	s64 delta;

	 
	if (!(ctx->nr_freq || needs_unthr))
		return;

	raw_spin_lock(&ctx->lock);
	perf_pmu_disable(ctx->pmu);

	list_for_each_entry_rcu(event, &ctx->event_list, event_entry) {
		if (event->state != PERF_EVENT_STATE_ACTIVE)
			continue;

		if (!event_filter_match(event))
			continue;

		hwc = &event->hw;

		if (needs_unthr && hwc->interrupts == MAX_INTERRUPTS) {
			hwc->interrupts = 0;
			perf_log_throttle(event, 1);
			event->pmu->start(event, 0);
		}

		if (!event->attr.freq || !event->attr.sample_freq)
			continue;

		 
		event->pmu->stop(event, PERF_EF_UPDATE);

		now = local64_read(&event->count);
		delta = now - hwc->freq_count_stamp;
		hwc->freq_count_stamp = now;

		 
		if (delta > 0)
			perf_adjust_period(event, period, delta, false);

		event->pmu->start(event, delta > 0 ? PERF_EF_RELOAD : 0);
	}

	perf_pmu_enable(ctx->pmu);
	raw_spin_unlock(&ctx->lock);
}
