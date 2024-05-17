static int fsl_emb_pmu_event_init(struct perf_event *event)
{
	u64 ev;
	struct perf_event *events[MAX_HWEVENTS];
	int n;
	int err;
	int num_restricted;
	int i;

	switch (event->attr.type) {
	case PERF_TYPE_HARDWARE:
		ev = event->attr.config;
		if (ev >= ppmu->n_generic || ppmu->generic_events[ev] == 0)
			return -EOPNOTSUPP;
		ev = ppmu->generic_events[ev];
		break;

	case PERF_TYPE_HW_CACHE:
		err = hw_perf_cache_event(event->attr.config, &ev);
		if (err)
			return err;
		break;

	case PERF_TYPE_RAW:
		ev = event->attr.config;
		break;

	default:
		return -ENOENT;
	}

	event->hw.config = ppmu->xlate_event(ev);
	if (!(event->hw.config & FSL_EMB_EVENT_VALID))
		return -EINVAL;

	 
	n = 0;
	if (event->group_leader != event) {
		n = collect_events(event->group_leader,
		                   ppmu->n_counter - 1, events);
		if (n < 0)
			return -EINVAL;
	}

	if (event->hw.config & FSL_EMB_EVENT_RESTRICTED) {
		num_restricted = 0;
		for (i = 0; i < n; i++) {
			if (events[i]->hw.config & FSL_EMB_EVENT_RESTRICTED)
				num_restricted++;
		}

		if (num_restricted >= ppmu->n_restricted)
			return -EINVAL;
	}

	event->hw.idx = -1;

	event->hw.config_base = PMLCA_CE | PMLCA_FCM1 |
	                        (u32)((ev << 16) & PMLCA_EVENT_MASK);

	if (event->attr.exclude_user)
		event->hw.config_base |= PMLCA_FCU;
	if (event->attr.exclude_kernel)
		event->hw.config_base |= PMLCA_FCS;
	if (event->attr.exclude_idle)
		return -ENOTSUPP;

	event->hw.last_period = event->hw.sample_period;
	local64_set(&event->hw.period_left, event->hw.last_period);

	 
	err = 0;
	if (!atomic_inc_not_zero(&num_events)) {
		mutex_lock(&pmc_reserve_mutex);
		if (atomic_read(&num_events) == 0 &&
		    reserve_pmc_hardware(perf_event_interrupt))
			err = -EBUSY;
		else
			atomic_inc(&num_events);
		mutex_unlock(&pmc_reserve_mutex);

		mtpmr(PMRN_PMGC0, PMGC0_FAC);
		isync();
	}
	event->destroy = hw_perf_event_destroy;

	return err;
}