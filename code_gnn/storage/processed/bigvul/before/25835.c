static void intel_pmu_drain_pebs_nhm(struct pt_regs *iregs)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	struct debug_store *ds = cpuc->ds;
	struct pebs_record_nhm *at, *top;
	struct perf_event *event = NULL;
	u64 status = 0;
	int bit, n;

	if (!x86_pmu.pebs_active)
		return;

	at  = (struct pebs_record_nhm *)(unsigned long)ds->pebs_buffer_base;
	top = (struct pebs_record_nhm *)(unsigned long)ds->pebs_index;

	ds->pebs_index = ds->pebs_buffer_base;

	n = top - at;
	if (n <= 0)
		return;

	 
	WARN_ON_ONCE(n > MAX_PEBS_EVENTS);

	for ( ; at < top; at++) {
		for_each_set_bit(bit, (unsigned long *)&at->status, MAX_PEBS_EVENTS) {
			event = cpuc->events[bit];
			if (!test_bit(bit, cpuc->active_mask))
				continue;

			WARN_ON_ONCE(!event);

			if (!event->attr.precise_ip)
				continue;

			if (__test_and_set_bit(bit, (unsigned long *)&status))
				continue;

			break;
		}

		if (!event || bit >= MAX_PEBS_EVENTS)
			continue;

		__intel_pmu_pebs_event(event, iregs, at);
	}
}