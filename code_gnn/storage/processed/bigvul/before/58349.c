void soft_restart(unsigned long addr)
{
	u64 *stack = soft_restart_stack + ARRAY_SIZE(soft_restart_stack);

	 
	local_irq_disable();
	local_fiq_disable();

	 
	if (num_online_cpus() == 1)
		outer_disable();

	 
	call_with_stack(__soft_restart, (void *)addr, (void *)stack);

	 
	BUG();
}
