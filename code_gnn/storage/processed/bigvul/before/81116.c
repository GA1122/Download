static void posix_cpu_timer_get(struct k_itimer *timer, struct itimerspec64 *itp)
{
	u64 now;
	struct task_struct *p = timer->it.cpu.task;

	WARN_ON_ONCE(p == NULL);

	 
	itp->it_interval = ns_to_timespec64(timer->it.cpu.incr);

	if (!timer->it.cpu.expires)
		return;

	 
	if (CPUCLOCK_PERTHREAD(timer->it_clock)) {
		cpu_clock_sample(timer->it_clock, p, &now);
	} else {
		struct sighand_struct *sighand;
		unsigned long flags;

		 
		sighand = lock_task_sighand(p, &flags);
		if (unlikely(sighand == NULL)) {
			 
			timer->it.cpu.expires = 0;
			return;
		} else {
			cpu_timer_sample_group(timer->it_clock, p, &now);
			unlock_task_sighand(p, &flags);
		}
	}

	if (now < timer->it.cpu.expires) {
		itp->it_value = ns_to_timespec64(timer->it.cpu.expires - now);
	} else {
		 
		itp->it_value.tv_nsec = 1;
		itp->it_value.tv_sec = 0;
	}
}
