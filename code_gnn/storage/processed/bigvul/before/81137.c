COMPAT_SYSCALL_DEFINE2(timer_gettime, timer_t, timer_id,
		       struct compat_itimerspec __user *, setting)
{
	struct itimerspec64 cur_setting;

	int ret = do_timer_gettime(timer_id, &cur_setting);
	if (!ret) {
		if (put_compat_itimerspec64(&cur_setting, setting))
			ret = -EFAULT;
	}
	return ret;
}
