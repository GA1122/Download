void touch_all_softlockup_watchdogs(void)
{
	int cpu;

	 
	for_each_online_cpu(cpu)
		per_cpu(watchdog_touch_ts, cpu) = 0;
}
