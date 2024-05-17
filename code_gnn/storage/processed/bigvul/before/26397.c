static void watchdog_interrupt_count(void)
{
	__this_cpu_inc(hrtimer_interrupts);
}
