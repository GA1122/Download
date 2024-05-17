posix_cpu_clock_set(const clockid_t which_clock, const struct timespec64 *tp)
{
	 
	int error = check_clock(which_clock);
	if (error == 0) {
		error = -EPERM;
	}
	return error;
}
