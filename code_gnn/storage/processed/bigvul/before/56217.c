static void armv8pmu_reset(void *info)
{
	u32 idx, nb_cnt = cpu_pmu->num_events;

	 
	for (idx = ARMV8_IDX_CYCLE_COUNTER; idx < nb_cnt; ++idx)
		armv8pmu_disable_event(NULL, idx);

	 
	armv8pmu_pmcr_write(ARMV8_PMCR_P | ARMV8_PMCR_C);

	 
	asm volatile("msr pmuserenr_el0, %0" :: "r" (0));
}
