COMPAT_SYSCALL_DEFINE4(timer_settime, timer_t, timer_id, int, flags,
		       struct compat_itimerspec __user *, new,
		       struct compat_itimerspec __user *, old)
{
	struct itimerspec64 new_spec, old_spec;
	struct itimerspec64 *rtn = old ? &old_spec : NULL;
	int error = 0;

	if (!new)
		return -EINVAL;
	if (get_compat_itimerspec64(&new_spec, new))
		return -EFAULT;

	error = do_timer_settime(timer_id, flags, &new_spec, rtn);
	if (!error && old) {
		if (put_compat_itimerspec64(&old_spec, old))
			error = -EFAULT;
	}
	return error;
}
