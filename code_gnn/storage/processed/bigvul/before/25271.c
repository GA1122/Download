static u32 __init armv7_read_num_pmnc_events(void)
{
	u32 nb_cnt;

	 
	nb_cnt = (armv7_pmnc_read() >> ARMV7_PMNC_N_SHIFT) & ARMV7_PMNC_N_MASK;

	 
	return nb_cnt + 1;
}