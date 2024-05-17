asmlinkage __visible void __sched notrace preempt_schedule(void)
{
	 
	if (likely(!preemptible()))
		return;

	preempt_schedule_common();
}
