static int fsl_emb_pmu_add(struct perf_event *event, int flags)
{
	struct cpu_hw_events *cpuhw;
	int ret = -EAGAIN;
	int num_counters = ppmu->n_counter;
	u64 val;
	int i;

	perf_pmu_disable(event->pmu);
	cpuhw = &get_cpu_var(cpu_hw_events);

	if (event->hw.config & FSL_EMB_EVENT_RESTRICTED)
		num_counters = ppmu->n_restricted;

	 
	for (i = num_counters - 1; i >= 0; i--) {
		if (cpuhw->event[i])
			continue;

		break;
	}

	if (i < 0)
		goto out;

	event->hw.idx = i;
	cpuhw->event[i] = event;
	++cpuhw->n_events;

	val = 0;
	if (event->hw.sample_period) {
		s64 left = local64_read(&event->hw.period_left);
		if (left < 0x80000000L)
			val = 0x80000000L - left;
	}
	local64_set(&event->hw.prev_count, val);

	if (!(flags & PERF_EF_START)) {
		event->hw.state = PERF_HES_STOPPED | PERF_HES_UPTODATE;
		val = 0;
	}

	write_pmc(i, val);
	perf_event_update_userpage(event);

	write_pmlcb(i, event->hw.config >> 32);
	write_pmlca(i, event->hw.config_base);

	ret = 0;
 out:
	put_cpu_var(cpu_hw_events);
	perf_pmu_enable(event->pmu);
	return ret;
}