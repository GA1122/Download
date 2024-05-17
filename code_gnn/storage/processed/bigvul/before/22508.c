asmlinkage void __sched notrace preempt_schedule(void)
{
	struct thread_info *ti = current_thread_info();

	 
	if (likely(ti->preempt_count || irqs_disabled()))
		return;

	do {
		add_preempt_count_notrace(PREEMPT_ACTIVE);
		schedule();
		sub_preempt_count_notrace(PREEMPT_ACTIVE);

		 
		barrier();
	} while (need_resched());
}
