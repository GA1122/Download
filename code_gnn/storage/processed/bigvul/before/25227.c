static void alpha_pmu_read(struct perf_event *event)
{
	struct hw_perf_event *hwc = &event->hw;

	alpha_perf_event_update(event, hwc, hwc->idx, 0);
}
