static int __perf_event_overflow(struct perf_event *event,
				   int throttle, struct perf_sample_data *data,
				   struct pt_regs *regs)
{
	int events = atomic_read(&event->event_limit);
	struct hw_perf_event *hwc = &event->hw;
	u64 seq;
	int ret = 0;

	 
	if (unlikely(!is_sampling_event(event)))
		return 0;

	seq = __this_cpu_read(perf_throttled_seq);
	if (seq != hwc->interrupts_seq) {
		hwc->interrupts_seq = seq;
		hwc->interrupts = 1;
	} else {
		hwc->interrupts++;
		if (unlikely(throttle
			     && hwc->interrupts >= max_samples_per_tick)) {
			__this_cpu_inc(perf_throttled_count);
			tick_dep_set_cpu(smp_processor_id(), TICK_DEP_BIT_PERF_EVENTS);
			hwc->interrupts = MAX_INTERRUPTS;
			perf_log_throttle(event, 0);
			ret = 1;
		}
	}

	if (event->attr.freq) {
		u64 now = perf_clock();
		s64 delta = now - hwc->freq_time_stamp;

		hwc->freq_time_stamp = now;

		if (delta > 0 && delta < 2*TICK_NSEC)
			perf_adjust_period(event, delta, hwc->last_period, true);
	}

	 

	event->pending_kill = POLL_IN;
	if (events && atomic_dec_and_test(&event->event_limit)) {
		ret = 1;
		event->pending_kill = POLL_HUP;

		perf_event_disable_inatomic(event);
	}

	READ_ONCE(event->overflow_handler)(event, data, regs);

	if (*perf_event_fasync(event) && event->pending_kill) {
		event->pending_wakeup = 1;
		irq_work_queue(&event->pending);
	}

	return ret;
}