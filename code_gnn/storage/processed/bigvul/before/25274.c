static int armv7pmu_get_event_idx(struct cpu_hw_events *cpuc,
				  struct hw_perf_event *event)
{
	int idx;

	 
	if (event->config_base == ARMV7_PERFCTR_CPU_CYCLES) {
		if (test_and_set_bit(ARMV7_CYCLE_COUNTER, cpuc->used_mask))
			return -EAGAIN;

		return ARMV7_CYCLE_COUNTER;
	} else {
		 
		for (idx = ARMV7_COUNTER0; idx <= armpmu->num_events; ++idx) {
			if (!test_and_set_bit(idx, cpuc->used_mask))
				return idx;
		}

		 
		return -EAGAIN;
	}
}
