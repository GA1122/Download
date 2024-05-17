armpmu_read(struct perf_event *event)
{
	struct hw_perf_event *hwc = &event->hw;

	 
	if (hwc->idx < 0)
		return;

	armpmu_event_update(event, hwc, hwc->idx);
}
