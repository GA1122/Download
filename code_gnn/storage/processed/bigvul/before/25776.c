x86_perf_event_set_period(struct perf_event *event)
{
	struct hw_perf_event *hwc = &event->hw;
	s64 left = local64_read(&hwc->period_left);
	s64 period = hwc->sample_period;
	int ret = 0, idx = hwc->idx;

	if (idx == X86_PMC_IDX_FIXED_BTS)
		return 0;

	 
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
	 
	if (unlikely(left < 2))
		left = 2;

	if (left > x86_pmu.max_period)
		left = x86_pmu.max_period;

	per_cpu(pmc_prev_left[idx], smp_processor_id()) = left;

	 
	local64_set(&hwc->prev_count, (u64)-left);

	wrmsrl(hwc->event_base, (u64)(-left) & x86_pmu.cntval_mask);

	 
	if (x86_pmu.perfctr_second_write) {
		wrmsrl(hwc->event_base,
			(u64)(-left) & x86_pmu.cntval_mask);
	}

	perf_event_update_userpage(event);

	return ret;
}