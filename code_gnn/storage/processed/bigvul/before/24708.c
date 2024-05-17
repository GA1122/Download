int do_sys_settimeofday(struct timespec *tv, struct timezone *tz)
{
	static int firsttime = 1;
	int error = 0;

	if (tv && !timespec_valid(tv))
		return -EINVAL;

	error = security_settime(tv, tz);
	if (error)
		return error;

	if (tz) {
		 
		sys_tz = *tz;
		update_vsyscall_tz();
		if (firsttime) {
			firsttime = 0;
			if (!tv)
				warp_clock();
		}
	}
	if (tv)
	{
		 
		return do_settimeofday(tv);
	}
	return 0;
}
