static int do_timerfd_settime(int ufd, int flags, 
		const struct itimerspec *new,
		struct itimerspec *old)
{
	struct fd f;
	struct timerfd_ctx *ctx;
	int ret;

	if ((flags & ~TFD_SETTIME_FLAGS) ||
	    !timespec_valid(&new->it_value) ||
	    !timespec_valid(&new->it_interval))
		return -EINVAL;

	ret = timerfd_fget(ufd, &f);
	if (ret)
		return ret;
	ctx = f.file->private_data;

	if (!capable(CAP_WAKE_ALARM) && isalarm(ctx)) {
		fdput(f);
		return -EPERM;
	}

	timerfd_setup_cancel(ctx, flags);

	 
	for (;;) {
		spin_lock_irq(&ctx->wqh.lock);

		if (isalarm(ctx)) {
			if (alarm_try_to_cancel(&ctx->t.alarm) >= 0)
				break;
		} else {
			if (hrtimer_try_to_cancel(&ctx->t.tmr) >= 0)
				break;
		}
		spin_unlock_irq(&ctx->wqh.lock);
		cpu_relax();
	}

	 
	if (ctx->expired && ctx->tintv) {
		if (isalarm(ctx))
			alarm_forward_now(&ctx->t.alarm, ctx->tintv);
		else
			hrtimer_forward_now(&ctx->t.tmr, ctx->tintv);
	}

	old->it_value = ktime_to_timespec(timerfd_get_remaining(ctx));
	old->it_interval = ktime_to_timespec(ctx->tintv);

	 
	ret = timerfd_setup(ctx, flags, new);

	spin_unlock_irq(&ctx->wqh.lock);
	fdput(f);
	return ret;
}