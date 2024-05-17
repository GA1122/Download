static void tty_update_time(struct timespec *time)
{
	unsigned long sec = get_seconds();

	 
	if ((sec ^ time->tv_sec) & ~7)
		time->tv_sec = sec;
}
