static void posix_cpu_timers_init_group(struct signal_struct *sig)
{
	 
	thread_group_cputime_init(sig);

	 
	sig->it_virt_expires = cputime_zero;
	sig->it_virt_incr = cputime_zero;
	sig->it_prof_expires = cputime_zero;
	sig->it_prof_incr = cputime_zero;

	 
	sig->cputime_expires.prof_exp = cputime_zero;
	sig->cputime_expires.virt_exp = cputime_zero;
	sig->cputime_expires.sched_exp = 0;

	 
	INIT_LIST_HEAD(&sig->cpu_timers[0]);
	INIT_LIST_HEAD(&sig->cpu_timers[1]);
	INIT_LIST_HEAD(&sig->cpu_timers[2]);
}