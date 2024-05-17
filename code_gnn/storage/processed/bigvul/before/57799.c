static cycle_t read_tsc(void)
{
	cycle_t ret = (cycle_t)rdtsc_ordered();
	u64 last = pvclock_gtod_data.clock.cycle_last;

	if (likely(ret >= last))
		return ret;

	 
	asm volatile ("");
	return last;
}
