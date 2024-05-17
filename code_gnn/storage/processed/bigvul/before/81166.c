static int do_timer_settime(timer_t timer_id, int flags,
			    struct itimerspec64 *new_spec64,
			    struct itimerspec64 *old_spec64)
{
	const struct k_clock *kc;
	struct k_itimer *timr;
	unsigned long flag;
	int error = 0;

	if (!timespec64_valid(&new_spec64->it_interval) ||
	    !timespec64_valid(&new_spec64->it_value))
		return -EINVAL;

	if (old_spec64)
		memset(old_spec64, 0, sizeof(*old_spec64));
retry:
	timr = lock_timer(timer_id, &flag);
	if (!timr)
		return -EINVAL;

	kc = timr->kclock;
	if (WARN_ON_ONCE(!kc || !kc->timer_set))
		error = -EINVAL;
	else
		error = kc->timer_set(timr, flags, new_spec64, old_spec64);

	unlock_timer(timr, flag);
	if (error == TIMER_RETRY) {
		old_spec64 = NULL;	 
		goto retry;
	}

	return error;
}