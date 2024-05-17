static void client_background(void)
{
	bb_daemonize(0);
	logmode &= ~LOGMODE_STDIO;
	 
	write_pidfile(client_config.pidfile);
}
