static void power_pmu_enable(struct pmu *pmu)
{
	struct perf_event *event;
	struct cpu_hw_events *cpuhw;
	unsigned long flags;
	long i;
	unsigned long val;
	s64 left;
	unsigned int hwc_index[MAX_HWEVENTS];
	int n_lim;
	int idx;

	if (!ppmu)
		return;
	local_irq_save(flags);
	cpuhw = &__get_cpu_var(cpu_hw_events);
	if (!cpuhw->disabled) {
		local_irq_restore(flags);
		return;
	}
	cpuhw->disabled = 0;

	 
	if (!cpuhw->n_added) {
		mtspr(SPRN_MMCRA, cpuhw->mmcr[2] & ~MMCRA_SAMPLE_ENABLE);
		mtspr(SPRN_MMCR1, cpuhw->mmcr[1]);
		if (cpuhw->n_events == 0)
			ppc_set_pmu_inuse(0);
		goto out_enable;
	}

	 
	if (ppmu->compute_mmcr(cpuhw->events, cpuhw->n_events, hwc_index,
			       cpuhw->mmcr)) {
		 
		printk(KERN_ERR "oops compute_mmcr failed\n");
		goto out;
	}

	 
	event = cpuhw->event[0];
	if (event->attr.exclude_user)
		cpuhw->mmcr[0] |= MMCR0_FCP;
	if (event->attr.exclude_kernel)
		cpuhw->mmcr[0] |= freeze_events_kernel;
	if (event->attr.exclude_hv)
		cpuhw->mmcr[0] |= MMCR0_FCHV;

	 
	ppc_set_pmu_inuse(1);
	mtspr(SPRN_MMCRA, cpuhw->mmcr[2] & ~MMCRA_SAMPLE_ENABLE);
	mtspr(SPRN_MMCR1, cpuhw->mmcr[1]);
	mtspr(SPRN_MMCR0, (cpuhw->mmcr[0] & ~(MMCR0_PMC1CE | MMCR0_PMCjCE))
				| MMCR0_FC);

	 
	for (i = 0; i < cpuhw->n_events; ++i) {
		event = cpuhw->event[i];
		if (event->hw.idx && event->hw.idx != hwc_index[i] + 1) {
			power_pmu_read(event);
			write_pmc(event->hw.idx, 0);
			event->hw.idx = 0;
		}
	}

	 
	cpuhw->n_limited = n_lim = 0;
	for (i = 0; i < cpuhw->n_events; ++i) {
		event = cpuhw->event[i];
		if (event->hw.idx)
			continue;
		idx = hwc_index[i] + 1;
		if (is_limited_pmc(idx)) {
			cpuhw->limited_counter[n_lim] = event;
			cpuhw->limited_hwidx[n_lim] = idx;
			++n_lim;
			continue;
		}
		val = 0;
		if (event->hw.sample_period) {
			left = local64_read(&event->hw.period_left);
			if (left < 0x80000000L)
				val = 0x80000000L - left;
		}
		local64_set(&event->hw.prev_count, val);
		event->hw.idx = idx;
		if (event->hw.state & PERF_HES_STOPPED)
			val = 0;
		write_pmc(idx, val);
		perf_event_update_userpage(event);
	}
	cpuhw->n_limited = n_lim;
	cpuhw->mmcr[0] |= MMCR0_PMXE | MMCR0_FCECE;

 out_enable:
	mb();
	write_mmcr0(cpuhw, cpuhw->mmcr[0]);

	 
	if (cpuhw->mmcr[2] & MMCRA_SAMPLE_ENABLE) {
		mb();
		mtspr(SPRN_MMCRA, cpuhw->mmcr[2]);
	}

 out:
	local_irq_restore(flags);
}