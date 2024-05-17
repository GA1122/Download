__init static int tracing_set_default_clock(void)
{
	 
	if (!trace_boot_clock && !sched_clock_stable()) {
		printk(KERN_WARNING
		       "Unstable clock detected, switching default tracing clock to \"global\"\n"
		       "If you want to keep using the local clock, then add:\n"
		       "  \"trace_clock=local\"\n"
		       "on the kernel command line\n");
		tracing_set_clock(&global_trace, "global");
	}

	return 0;
}