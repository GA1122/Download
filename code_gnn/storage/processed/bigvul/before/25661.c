static int sparc_pmu_add(struct perf_event *event, int ef_flags)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	int n0, ret = -EAGAIN;
	unsigned long flags;

	local_irq_save(flags);
	perf_pmu_disable(event->pmu);

	n0 = cpuc->n_events;
	if (n0 >= MAX_HWEVENTS)
		goto out;

	cpuc->event[n0] = event;
	cpuc->events[n0] = event->hw.event_base;
	cpuc->current_idx[n0] = PIC_NO_INDEX;

	event->hw.state = PERF_HES_UPTODATE;
	if (!(ef_flags & PERF_EF_START))
		event->hw.state |= PERF_HES_STOPPED;

	 
	if (cpuc->group_flag & PERF_EVENT_TXN)
		goto nocheck;

	if (check_excludes(cpuc->event, n0, 1))
		goto out;
	if (sparc_check_constraints(cpuc->event, cpuc->events, n0 + 1))
		goto out;

nocheck:
	cpuc->n_events++;
	cpuc->n_added++;

	ret = 0;
out:
	perf_pmu_enable(event->pmu);
	local_irq_restore(flags);
	return ret;
}
