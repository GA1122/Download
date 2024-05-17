static void run_ksoftirqd(unsigned int cpu)
{
	local_irq_disable();
	if (local_softirq_pending()) {
		 
		__do_softirq();
		local_irq_enable();
		cond_resched();
		return;
	}
	local_irq_enable();
}
