event_sched_in(struct perf_event *event,
		 struct perf_cpu_context *cpuctx,
		 struct perf_event_context *ctx)
{
	u64 tstamp = perf_event_time(event);
	int ret = 0;

	lockdep_assert_held(&ctx->lock);

	if (event->state <= PERF_EVENT_STATE_OFF)
		return 0;

	event->state = PERF_EVENT_STATE_ACTIVE;
	event->oncpu = smp_processor_id();

	 
	if (unlikely(event->hw.interrupts == MAX_INTERRUPTS)) {
		perf_log_throttle(event, 1);
		event->hw.interrupts = 0;
	}

	 
	smp_wmb();

	perf_pmu_disable(event->pmu);

	perf_set_shadow_time(event, ctx, tstamp);

	perf_log_itrace_start(event);

	if (event->pmu->add(event, PERF_EF_START)) {
		event->state = PERF_EVENT_STATE_INACTIVE;
		event->oncpu = -1;
		ret = -EAGAIN;
		goto out;
	}

	event->tstamp_running += tstamp - event->tstamp_stopped;

	if (!is_software_event(event))
		cpuctx->active_oncpu++;
	if (!ctx->nr_active++)
		perf_event_ctx_activate(ctx);
	if (event->attr.freq && event->attr.sample_freq)
		ctx->nr_freq++;

	if (event->attr.exclusive)
		cpuctx->exclusive = 1;

	if (is_orphaned_child(event))
		schedule_orphans_remove(ctx);

out:
	perf_pmu_enable(event->pmu);

	return ret;
}