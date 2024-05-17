static const struct arm_pmu *__init armv7_a9_pmu_init(void)
{
	armv7pmu.id		= ARM_PERF_PMU_ID_CA9;
	armv7pmu.name		= "ARMv7 Cortex-A9";
	armv7pmu.cache_map	= &armv7_a9_perf_cache_map;
	armv7pmu.event_map	= &armv7_a9_perf_map;
	armv7pmu.num_events	= armv7_read_num_pmnc_events();
	return &armv7pmu;
}
