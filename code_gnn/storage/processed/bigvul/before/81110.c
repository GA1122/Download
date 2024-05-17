posix_cpu_clock_getres(const clockid_t which_clock, struct timespec64 *tp)
{
	int error = check_clock(which_clock);
	if (!error) {
		tp->tv_sec = 0;
		tp->tv_nsec = ((NSEC_PER_SEC + HZ - 1) / HZ);
		if (CPUCLOCK_WHICH(which_clock) == CPUCLOCK_SCHED) {
			 
			tp->tv_nsec = 1;
		}
	}
	return error;
}
