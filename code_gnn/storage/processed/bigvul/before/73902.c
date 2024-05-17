sigterm(int sig)
{
	(void)sig;
	 
	 
#if 0
	 
	syslog(LOG_NOTICE, "received signal %d, good-bye", sig);
#endif
	quitting = 1;
	 
}
