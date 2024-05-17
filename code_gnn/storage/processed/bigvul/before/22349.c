static void calc_global_nohz(unsigned long ticks)
{
	long delta, active, n;

	if (time_before(jiffies, calc_load_update))
		return;

	 
	delta = calc_load_fold_idle();
	if (delta)
		atomic_long_add(delta, &calc_load_tasks);

	 
	if (ticks >= LOAD_FREQ) {
		n = ticks / LOAD_FREQ;

		active = atomic_long_read(&calc_load_tasks);
		active = active > 0 ? active * FIXED_1 : 0;

		avenrun[0] = calc_load_n(avenrun[0], EXP_1, active, n);
		avenrun[1] = calc_load_n(avenrun[1], EXP_5, active, n);
		avenrun[2] = calc_load_n(avenrun[2], EXP_15, active, n);

		calc_load_update += n * LOAD_FREQ;
	}

	 
}
