asmlinkage void __sched preempt_schedule_irq(void)
{
	enum ctx_state prev_state;

	 
	BUG_ON(preempt_count() || !irqs_disabled());

	prev_state = exception_enter();

	do {
		__preempt_count_add(PREEMPT_ACTIVE);
		local_irq_enable();
		__schedule();
		local_irq_disable();
		__preempt_count_sub(PREEMPT_ACTIVE);

		 
		barrier();
	} while (need_resched());

	exception_exit(prev_state);
}
