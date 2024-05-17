static int armv8pmu_get_event_idx(struct pmu_hw_events *cpuc,
				  struct hw_perf_event *event)
{
	int idx;
	unsigned long evtype = event->config_base & ARMV8_EVTYPE_EVENT;

	 
	if (evtype == ARMV8_PMUV3_PERFCTR_CLOCK_CYCLES) {
		if (test_and_set_bit(ARMV8_IDX_CYCLE_COUNTER, cpuc->used_mask))
			return -EAGAIN;

		return ARMV8_IDX_CYCLE_COUNTER;
	}

	 
	for (idx = ARMV8_IDX_COUNTER0; idx < cpu_pmu->num_events; ++idx) {
		if (!test_and_set_bit(idx, cpuc->used_mask))
			return idx;
	}

	 
	return -EAGAIN;
}
