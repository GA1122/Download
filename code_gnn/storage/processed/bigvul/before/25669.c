static int sparc_pmu_event_init(struct perf_event *event)
{
	struct perf_event_attr *attr = &event->attr;
	struct perf_event *evts[MAX_HWEVENTS];
	struct hw_perf_event *hwc = &event->hw;
	unsigned long events[MAX_HWEVENTS];
	int current_idx_dmy[MAX_HWEVENTS];
	const struct perf_event_map *pmap;
	int n;

	if (atomic_read(&nmi_active) < 0)
		return -ENODEV;

	switch (attr->type) {
	case PERF_TYPE_HARDWARE:
		if (attr->config >= sparc_pmu->max_events)
			return -EINVAL;
		pmap = sparc_pmu->event_map(attr->config);
		break;

	case PERF_TYPE_HW_CACHE:
		pmap = sparc_map_cache_event(attr->config);
		if (IS_ERR(pmap))
			return PTR_ERR(pmap);
		break;

	case PERF_TYPE_RAW:
		pmap = NULL;
		break;

	default:
		return -ENOENT;

	}

	if (pmap) {
		hwc->event_base = perf_event_encode(pmap);
	} else {
		 
		hwc->event_base = attr->config;
	}

	 
	hwc->config_base = sparc_pmu->irq_bit;
	if (!attr->exclude_user)
		hwc->config_base |= PCR_UTRACE;
	if (!attr->exclude_kernel)
		hwc->config_base |= PCR_STRACE;
	if (!attr->exclude_hv)
		hwc->config_base |= sparc_pmu->hv_bit;

	n = 0;
	if (event->group_leader != event) {
		n = collect_events(event->group_leader,
				   MAX_HWEVENTS - 1,
				   evts, events, current_idx_dmy);
		if (n < 0)
			return -EINVAL;
	}
	events[n] = hwc->event_base;
	evts[n] = event;

	if (check_excludes(evts, n, 1))
		return -EINVAL;

	if (sparc_check_constraints(evts, events, n + 1))
		return -EINVAL;

	hwc->idx = PIC_NO_INDEX;

	 
	perf_event_grab_pmc();
	event->destroy = hw_perf_event_destroy;

	if (!hwc->sample_period) {
		hwc->sample_period = MAX_PERIOD;
		hwc->last_period = hwc->sample_period;
		local64_set(&hwc->period_left, hwc->sample_period);
	}

	return 0;
}
