static inline void invoke_softirq(void)
{
	if (ksoftirqd_running())
		return;

	if (!force_irqthreads) {
#ifdef CONFIG_HAVE_IRQ_EXIT_ON_IRQ_STACK
		 
		__do_softirq();
#else
		 
		do_softirq_own_stack();
#endif
	} else {
		wakeup_softirqd();
	}
}
