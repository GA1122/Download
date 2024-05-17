static void armv7pmu_reset(void *info)
{
	u32 idx, nb_cnt = armpmu->num_events;

	 
	for (idx = 1; idx < nb_cnt; ++idx)
		armv7pmu_disable_event(NULL, idx);

	 
	armv7_pmnc_write(ARMV7_PMNC_P | ARMV7_PMNC_C);
}
