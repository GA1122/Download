static cycle_t read_tsc(void)
{
	cycle_t ret;
	u64 last;

	 
	rdtsc_barrier();
	ret = (cycle_t)vget_cycles();

	last = pvclock_gtod_data.clock.cycle_last;

	if (likely(ret >= last))
		return ret;

	 
	asm volatile ("");
	return last;
}
