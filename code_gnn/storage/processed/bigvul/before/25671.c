static void sparc_pmu_start(struct perf_event *event, int flags)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	int idx = active_event_index(cpuc, event);

	if (flags & PERF_EF_RELOAD) {
		WARN_ON_ONCE(!(event->hw.state & PERF_HES_UPTODATE));
		sparc_perf_event_set_period(event, &event->hw, idx);
	}

	event->hw.state = 0;

	sparc_pmu_enable_event(cpuc, &event->hw, idx);
}
