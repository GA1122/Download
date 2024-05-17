static void sparc_pmu_stop(struct perf_event *event, int flags)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	int idx = active_event_index(cpuc, event);

	if (!(event->hw.state & PERF_HES_STOPPED)) {
		sparc_pmu_disable_event(cpuc, &event->hw, idx);
		event->hw.state |= PERF_HES_STOPPED;
	}

	if (!(event->hw.state & PERF_HES_UPTODATE) && (flags & PERF_EF_UPDATE)) {
		sparc_perf_event_update(event, &event->hw, idx);
		event->hw.state |= PERF_HES_UPTODATE;
	}
}
