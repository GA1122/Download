static void check_process_timers(struct task_struct *tsk,
				 struct list_head *firing)
{
	struct signal_struct *const sig = tsk->signal;
	u64 utime, ptime, virt_expires, prof_expires;
	u64 sum_sched_runtime, sched_expires;
	struct list_head *timers = sig->cpu_timers;
	struct task_cputime cputime;
	unsigned long soft;

	if (dl_task(tsk))
		check_dl_overrun(tsk);

	 
	if (!READ_ONCE(tsk->signal->cputimer.running))
		return;

         
	sig->cputimer.checking_timer = true;

	 
	thread_group_cputimer(tsk, &cputime);
	utime = cputime.utime;
	ptime = utime + cputime.stime;
	sum_sched_runtime = cputime.sum_exec_runtime;

	prof_expires = check_timers_list(timers, firing, ptime);
	virt_expires = check_timers_list(++timers, firing, utime);
	sched_expires = check_timers_list(++timers, firing, sum_sched_runtime);

	 
	check_cpu_itimer(tsk, &sig->it[CPUCLOCK_PROF], &prof_expires, ptime,
			 SIGPROF);
	check_cpu_itimer(tsk, &sig->it[CPUCLOCK_VIRT], &virt_expires, utime,
			 SIGVTALRM);
	soft = task_rlimit(tsk, RLIMIT_CPU);
	if (soft != RLIM_INFINITY) {
		unsigned long psecs = div_u64(ptime, NSEC_PER_SEC);
		unsigned long hard = task_rlimit_max(tsk, RLIMIT_CPU);
		u64 x;
		if (psecs >= hard) {
			 
			if (print_fatal_signals) {
				pr_info("RT Watchdog Timeout (hard): %s[%d]\n",
					tsk->comm, task_pid_nr(tsk));
			}
			__group_send_sig_info(SIGKILL, SEND_SIG_PRIV, tsk);
			return;
		}
		if (psecs >= soft) {
			 
			if (print_fatal_signals) {
				pr_info("CPU Watchdog Timeout (soft): %s[%d]\n",
					tsk->comm, task_pid_nr(tsk));
			}
			__group_send_sig_info(SIGXCPU, SEND_SIG_PRIV, tsk);
			if (soft < hard) {
				soft++;
				sig->rlim[RLIMIT_CPU].rlim_cur = soft;
			}
		}
		x = soft * NSEC_PER_SEC;
		if (!prof_expires || x < prof_expires)
			prof_expires = x;
	}

	sig->cputime_expires.prof_exp = prof_expires;
	sig->cputime_expires.virt_exp = virt_expires;
	sig->cputime_expires.sched_exp = sched_expires;
	if (task_cputime_zero(&sig->cputime_expires))
		stop_process_timers(sig);

	sig->cputimer.checking_timer = false;
}