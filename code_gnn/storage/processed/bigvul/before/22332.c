void __kprobes add_preempt_count(int val)
{
#ifdef CONFIG_DEBUG_PREEMPT
	 
	if (DEBUG_LOCKS_WARN_ON((preempt_count() < 0)))
		return;
#endif
	preempt_count() += val;
#ifdef CONFIG_DEBUG_PREEMPT
	 
	DEBUG_LOCKS_WARN_ON((preempt_count() & PREEMPT_MASK) >=
				PREEMPT_MASK - 10);
#endif
	if (preempt_count() == val)
		trace_preempt_off(CALLER_ADDR0, get_parent_ip(CALLER_ADDR1));
}