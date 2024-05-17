static int x86_setup_perfctr(struct perf_event *event)
{
	struct perf_event_attr *attr = &event->attr;
	struct hw_perf_event *hwc = &event->hw;
	u64 config;

	if (!is_sampling_event(event)) {
		hwc->sample_period = x86_pmu.max_period;
		hwc->last_period = hwc->sample_period;
		local64_set(&hwc->period_left, hwc->sample_period);
	} else {
		 
		if (!x86_pmu.apic)
			return -EOPNOTSUPP;
	}

	 
	if (attr->type == PERF_TYPE_RAW)
		return 0;

	if (attr->type == PERF_TYPE_HW_CACHE)
		return set_ext_hw_attr(hwc, event);

	if (attr->config >= x86_pmu.max_events)
		return -EINVAL;

	 
	config = x86_pmu.event_map(attr->config);

	if (config == 0)
		return -ENOENT;

	if (config == -1LL)
		return -EINVAL;

	 
	if (attr->config == PERF_COUNT_HW_BRANCH_INSTRUCTIONS &&
	    !attr->freq && hwc->sample_period == 1) {
		 
		if (!x86_pmu.bts_active)
			return -EOPNOTSUPP;

		 
		if (!attr->exclude_kernel)
			return -EOPNOTSUPP;
	}

	hwc->config |= config;

	return 0;
}
