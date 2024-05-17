static void __touch_watchdog(void)
{
	int this_cpu = smp_processor_id();

	__this_cpu_write(watchdog_touch_ts, get_timestamp(this_cpu));
}
