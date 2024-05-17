static int validate_event(struct cpu_hw_events *cpuc,
	       struct perf_event *event)
{
	struct hw_perf_event fake_hwc = event->hw;

	 
	if (event->pmu != &pmu || event->state <= PERF_EVENT_STATE_OFF)
		return 1;

	return mipspmu->alloc_counter(cpuc, &fake_hwc) >= 0;
}
