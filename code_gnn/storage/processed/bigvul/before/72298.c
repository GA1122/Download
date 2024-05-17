sighup_restart(void)
{
	logit("Received SIGHUP; restarting.");
	close_listen_socks();
	close_startup_pipes();
	alarm(0);   
	signal(SIGHUP, SIG_IGN);  
	execv(saved_argv[0], saved_argv);
	logit("RESTART FAILED: av[0]='%.100s', error: %.100s.", saved_argv[0],
	    strerror(errno));
	exit(1);
}
