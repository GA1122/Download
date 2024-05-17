ntpd_time_stepped(void)
{
	u_int saved_mon_enabled;

	 
	if (MON_OFF != mon_enabled) {
		saved_mon_enabled = mon_enabled;
		mon_stop(MON_OFF);
		mon_start(saved_mon_enabled);
	}

	 
#ifdef SYS_WINNT
	win_time_stepped();
#endif
}
