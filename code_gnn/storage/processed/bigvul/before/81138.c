COMPAT_SYSCALL_DEFINE2(clock_settime, clockid_t, which_clock,
		       struct compat_timespec __user *, tp)
{
	const struct k_clock *kc = clockid_to_kclock(which_clock);
	struct timespec64 ts;

	if (!kc || !kc->clock_set)
		return -EINVAL;

	if (compat_get_timespec64(&ts, tp))
		return -EFAULT;

	return kc->clock_set(which_clock, &ts);
}
