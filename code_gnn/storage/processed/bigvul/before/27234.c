SYSCALL_DEFINE5(osf_select, int, n, fd_set __user *, inp, fd_set __user *, outp,
		fd_set __user *, exp, struct timeval32 __user *, tvp)
{
	struct timespec end_time, *to = NULL;
	if (tvp) {
		time_t sec, usec;

		to = &end_time;

		if (!access_ok(VERIFY_READ, tvp, sizeof(*tvp))
		    || __get_user(sec, &tvp->tv_sec)
		    || __get_user(usec, &tvp->tv_usec)) {
		    	return -EFAULT;
		}

		if (sec < 0 || usec < 0)
			return -EINVAL;

		if (poll_select_set_timeout(to, sec, usec * NSEC_PER_USEC))
			return -EINVAL;		

	}

	 
	return core_sys_select(n, inp, outp, exp, to);
}