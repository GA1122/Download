void touch_softlockup_watchdog(void)
{
	__this_cpu_write(watchdog_touch_ts, 0);
}
