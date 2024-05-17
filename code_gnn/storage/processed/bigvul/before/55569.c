void preempt_count_sub(int val)
{
#ifdef CONFIG_DEBUG_PREEMPT
	 
	if (DEBUG_LOCKS_WARN_ON(val > preempt_count()))
		return;
	 
	if (DEBUG_LOCKS_WARN_ON((val < PREEMPT_MASK) &&
			!(preempt_count() & PREEMPT_MASK)))
		return;
#endif

	preempt_latency_stop(val);
	__preempt_count_sub(val);
}