static u32 __init armv8pmu_read_num_pmnc_events(void)
{
	u32 nb_cnt;

	 
	nb_cnt = (armv8pmu_pmcr_read() >> ARMV8_PMCR_N_SHIFT) & ARMV8_PMCR_N_MASK;

	 
	return nb_cnt + 1;
}
