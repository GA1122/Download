asmlinkage void __sched notrace preempt_schedule(void)
{
	 
	if (likely(!preemptible()))
		return;

	do {
		__preempt_count_add(PREEMPT_ACTIVE);
		__schedule();
		__preempt_count_sub(PREEMPT_ACTIVE);

		 
		barrier();
	} while (need_resched());
}
