mipspmu_event_set_period(struct perf_event *event,
			struct hw_perf_event *hwc,
			int idx)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	s64 left = local64_read(&hwc->period_left);
	s64 period = hwc->sample_period;
	int ret = 0;
	u64 uleft;
	unsigned long flags;

	if (unlikely(left <= -period)) {
		left = period;
		local64_set(&hwc->period_left, left);
		hwc->last_period = period;
		ret = 1;
	}

	if (unlikely(left <= 0)) {
		left += period;
		local64_set(&hwc->period_left, left);
		hwc->last_period = period;
		ret = 1;
	}

	if (left > (s64)MAX_PERIOD)
		left = MAX_PERIOD;

	local64_set(&hwc->prev_count, (u64)-left);

	local_irq_save(flags);
	uleft = (u64)(-left) & MAX_PERIOD;
	uleft > VALID_COUNT ?
		set_bit(idx, cpuc->msbs) : clear_bit(idx, cpuc->msbs);
	mipspmu->write_counter(idx, (u64)(-left) & VALID_COUNT);
	local_irq_restore(flags);

	perf_event_update_userpage(event);

	return ret;
}
