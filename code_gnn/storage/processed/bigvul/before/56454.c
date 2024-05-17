void tm_recheckpoint(struct thread_struct *thread,
		     unsigned long orig_msr)
{
	unsigned long flags;

	 
	local_irq_save(flags);
	hard_irq_disable();

	 
	tm_restore_sprs(thread);

	__tm_recheckpoint(thread, orig_msr);

	local_irq_restore(flags);
}
