void ist_begin_non_atomic(struct pt_regs *regs)
{
	BUG_ON(!user_mode(regs));

	 
	BUG_ON(!on_thread_stack());

	preempt_enable_no_resched();
}
