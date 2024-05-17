static long calc_load_fold_idle(void)
{
	long delta = 0;

	 
	if (atomic_long_read(&calc_load_tasks_idle))
		delta = atomic_long_xchg(&calc_load_tasks_idle, 0);

	return delta;
}
