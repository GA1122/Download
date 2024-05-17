static inline void stop_process_timers(struct signal_struct *sig)
{
	struct thread_group_cputimer *cputimer = &sig->cputimer;

	 
	WRITE_ONCE(cputimer->running, false);
	tick_dep_clear_signal(sig, TICK_DEP_BIT_POSIX_TIMER);
}