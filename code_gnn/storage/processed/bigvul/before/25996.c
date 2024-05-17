event_sched_in(struct perf_event *event,
		 struct perf_cpu_context *cpuctx,
		 struct perf_event_context *ctx)
{
	u64 tstamp = perf_event_time(event);

	if (event->state <= PERF_EVENT_STATE_OFF)
		return 0;

	event->state = PERF_EVENT_STATE_ACTIVE;
	event->oncpu = smp_processor_id();

	 
	if (unlikely(event->hw.interrupts == MAX_INTERRUPTS)) {
		perf_log_throttle(event, 1);
		event->hw.interrupts = 0;
	}

	 
	smp_wmb();

	if (event->pmu->add(event, PERF_EF_START)) {
		event->state = PERF_EVENT_STATE_INACTIVE;
		event->oncpu = -1;
		return -EAGAIN;
	}

	event->tstamp_running += tstamp - event->tstamp_stopped;

	perf_set_shadow_time(event, ctx, tstamp);

	if (!is_software_event(event))
		cpuctx->active_oncpu++;
	ctx->nr_active++;

	if (event->attr.exclusive)
		cpuctx->exclusive = 1;

	return 0;
}
