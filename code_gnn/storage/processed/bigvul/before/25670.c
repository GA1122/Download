static void sparc_pmu_read(struct perf_event *event)
{
	struct cpu_hw_events *cpuc = &__get_cpu_var(cpu_hw_events);
	int idx = active_event_index(cpuc, event);
	struct hw_perf_event *hwc = &event->hw;

	sparc_perf_event_update(event, hwc, idx);
}
