armv6pmu_get_event_idx(struct cpu_hw_events *cpuc,
		       struct hw_perf_event *event)
{
	 
	if (ARMV6_PERFCTR_CPU_CYCLES == event->config_base) {
		if (test_and_set_bit(ARMV6_CYCLE_COUNTER, cpuc->used_mask))
			return -EAGAIN;

		return ARMV6_CYCLE_COUNTER;
	} else {
		 
		if (!test_and_set_bit(ARMV6_COUNTER1, cpuc->used_mask))
			return ARMV6_COUNTER1;

		if (!test_and_set_bit(ARMV6_COUNTER0, cpuc->used_mask))
			return ARMV6_COUNTER0;

		 
		return -EAGAIN;
	}
}
