void set_process_cpu_timer(struct task_struct *tsk, unsigned int clock_idx,
			   u64 *newval, u64 *oldval)
{
	u64 now;
	int ret;

	WARN_ON_ONCE(clock_idx == CPUCLOCK_SCHED);
	ret = cpu_timer_sample_group(clock_idx, tsk, &now);

	if (oldval && ret != -EINVAL) {
		 
		if (*oldval) {
			if (*oldval <= now) {
				 
				*oldval = TICK_NSEC;
			} else {
				*oldval -= now;
			}
		}

		if (!*newval)
			return;
		*newval += now;
	}

	 
	switch (clock_idx) {
	case CPUCLOCK_PROF:
		if (expires_gt(tsk->signal->cputime_expires.prof_exp, *newval))
			tsk->signal->cputime_expires.prof_exp = *newval;
		break;
	case CPUCLOCK_VIRT:
		if (expires_gt(tsk->signal->cputime_expires.virt_exp, *newval))
			tsk->signal->cputime_expires.virt_exp = *newval;
		break;
	}

	tick_dep_set_signal(tsk->signal, TICK_DEP_BIT_POSIX_TIMER);
}
