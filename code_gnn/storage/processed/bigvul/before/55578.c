static void __sched notrace preempt_schedule_common(void)
{
	do {
		 
		preempt_disable_notrace();
		preempt_latency_start(1);
		__schedule(true);
		preempt_latency_stop(1);
		preempt_enable_no_resched_notrace();

		 
	} while (need_resched());
}
