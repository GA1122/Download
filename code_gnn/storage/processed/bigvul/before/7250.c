void fastcgi_cleanup(int signal)
{
#ifdef DEBUG_FASTCGI
	fprintf(stderr, "FastCGI shutdown, pid %d\n", getpid());
#endif

	sigaction(SIGTERM, &old_term, 0);

	 
	kill(-pgroup, SIGTERM);

	if (parent && parent_waiting) {
		exit_signal = 1;
	} else {
		exit(0);
	}
}