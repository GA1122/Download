static int posix_cpu_timer_set(struct k_itimer *timer, int timer_flags,
			       struct itimerspec64 *new, struct itimerspec64 *old)
{
	unsigned long flags;
	struct sighand_struct *sighand;
	struct task_struct *p = timer->it.cpu.task;
	u64 old_expires, new_expires, old_incr, val;
	int ret;

	WARN_ON_ONCE(p == NULL);

	 
	new_expires = ktime_to_ns(timespec64_to_ktime(new->it_value));

	 
	sighand = lock_task_sighand(p, &flags);
	 
	if (unlikely(sighand == NULL)) {
		return -ESRCH;
	}

	 
	lockdep_assert_irqs_disabled();

	ret = 0;
	old_incr = timer->it.cpu.incr;
	old_expires = timer->it.cpu.expires;
	if (unlikely(timer->it.cpu.firing)) {
		timer->it.cpu.firing = -1;
		ret = TIMER_RETRY;
	} else
		list_del_init(&timer->it.cpu.entry);

	 
	if (CPUCLOCK_PERTHREAD(timer->it_clock)) {
		cpu_clock_sample(timer->it_clock, p, &val);
	} else {
		cpu_timer_sample_group(timer->it_clock, p, &val);
	}

	if (old) {
		if (old_expires == 0) {
			old->it_value.tv_sec = 0;
			old->it_value.tv_nsec = 0;
		} else {
			 
			bump_cpu_timer(timer, val);
			if (val < timer->it.cpu.expires) {
				old_expires = timer->it.cpu.expires - val;
				old->it_value = ns_to_timespec64(old_expires);
			} else {
				old->it_value.tv_nsec = 1;
				old->it_value.tv_sec = 0;
			}
		}
	}

	if (unlikely(ret)) {
		 
		unlock_task_sighand(p, &flags);
		goto out;
	}

	if (new_expires != 0 && !(timer_flags & TIMER_ABSTIME)) {
		new_expires += val;
	}

	 
	timer->it.cpu.expires = new_expires;
	if (new_expires != 0 && val < new_expires) {
		arm_timer(timer);
	}

	unlock_task_sighand(p, &flags);
	 
	timer->it.cpu.incr = timespec64_to_ns(&new->it_interval);

	 
	timer->it_requeue_pending = (timer->it_requeue_pending + 2) &
		~REQUEUE_PENDING;
	timer->it_overrun_last = 0;
	timer->it_overrun = -1;

	if (new_expires != 0 && !(val < new_expires)) {
		 
		cpu_timer_fire(timer);
	}

	ret = 0;
 out:
	if (old)
		old->it_interval = ns_to_timespec64(old_incr);

	return ret;
}
