static void check_thread_timers(struct task_struct *tsk,
				struct list_head *firing)
{
	struct list_head *timers = tsk->cpu_timers;
	struct task_cputime *tsk_expires = &tsk->cputime_expires;
	u64 expires;
	unsigned long soft;

	if (dl_task(tsk))
		check_dl_overrun(tsk);

	 
	if (task_cputime_zero(&tsk->cputime_expires))
		return;

	expires = check_timers_list(timers, firing, prof_ticks(tsk));
	tsk_expires->prof_exp = expires;

	expires = check_timers_list(++timers, firing, virt_ticks(tsk));
	tsk_expires->virt_exp = expires;

	tsk_expires->sched_exp = check_timers_list(++timers, firing,
						   tsk->se.sum_exec_runtime);

	 
	soft = task_rlimit(tsk, RLIMIT_RTTIME);
	if (soft != RLIM_INFINITY) {
		unsigned long hard = task_rlimit_max(tsk, RLIMIT_RTTIME);

		if (hard != RLIM_INFINITY &&
		    tsk->rt.timeout > DIV_ROUND_UP(hard, USEC_PER_SEC/HZ)) {
			 
			if (print_fatal_signals) {
				pr_info("CPU Watchdog Timeout (hard): %s[%d]\n",
					tsk->comm, task_pid_nr(tsk));
			}
			__group_send_sig_info(SIGKILL, SEND_SIG_PRIV, tsk);
			return;
		}
		if (tsk->rt.timeout > DIV_ROUND_UP(soft, USEC_PER_SEC/HZ)) {
			 
			if (soft < hard) {
				soft += USEC_PER_SEC;
				tsk->signal->rlim[RLIMIT_RTTIME].rlim_cur =
					soft;
			}
			if (print_fatal_signals) {
				pr_info("RT Watchdog Timeout (soft): %s[%d]\n",
					tsk->comm, task_pid_nr(tsk));
			}
			__group_send_sig_info(SIGXCPU, SEND_SIG_PRIV, tsk);
		}
	}
	if (task_cputime_zero(tsk_expires))
		tick_dep_clear_task(tsk, TICK_DEP_BIT_POSIX_TIMER);
}