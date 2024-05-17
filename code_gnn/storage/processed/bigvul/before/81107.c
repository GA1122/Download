static inline int fastpath_timer_check(struct task_struct *tsk)
{
	struct signal_struct *sig;

	if (!task_cputime_zero(&tsk->cputime_expires)) {
		struct task_cputime task_sample;

		task_cputime(tsk, &task_sample.utime, &task_sample.stime);
		task_sample.sum_exec_runtime = tsk->se.sum_exec_runtime;
		if (task_cputime_expired(&task_sample, &tsk->cputime_expires))
			return 1;
	}

	sig = tsk->signal;
	 
	if (READ_ONCE(sig->cputimer.running) &&
	    !READ_ONCE(sig->cputimer.checking_timer)) {
		struct task_cputime group_sample;

		sample_cputime_atomic(&group_sample, &sig->cputimer.cputime_atomic);

		if (task_cputime_expired(&group_sample, &sig->cputime_expires))
			return 1;
	}

	if (dl_task(tsk) && tsk->dl.dl_overrun)
		return 1;

	return 0;
}
