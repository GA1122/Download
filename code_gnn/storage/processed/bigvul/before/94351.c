static void posix_cpu_timers_init_group(struct signal_struct *sig)
{
	 
	thread_group_cputime_init(sig);

	 
	sig->it[CPUCLOCK_PROF].expires = cputime_zero;
	sig->it[CPUCLOCK_PROF].incr = cputime_zero;
	sig->it[CPUCLOCK_VIRT].expires = cputime_zero;
	sig->it[CPUCLOCK_VIRT].incr = cputime_zero;

	 
	sig->cputime_expires.prof_exp = cputime_zero;
	sig->cputime_expires.virt_exp = cputime_zero;
	sig->cputime_expires.sched_exp = 0;

	if (sig->rlim[RLIMIT_CPU].rlim_cur != RLIM_INFINITY) {
		sig->cputime_expires.prof_exp =
			secs_to_cputime(sig->rlim[RLIMIT_CPU].rlim_cur);
		sig->cputimer.running = 1;
	}

	 
	INIT_LIST_HEAD(&sig->cpu_timers[0]);
	INIT_LIST_HEAD(&sig->cpu_timers[1]);
	INIT_LIST_HEAD(&sig->cpu_timers[2]);
}
