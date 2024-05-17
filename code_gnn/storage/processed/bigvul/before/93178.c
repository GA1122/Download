cleanup(int signo _U_)
{
#ifdef USE_WIN32_MM_TIMER
	if (timer_id)
		timeKillEvent(timer_id);
	timer_id = 0;
#elif defined(HAVE_ALARM)
	alarm(0);
#endif

#ifdef HAVE_PCAP_BREAKLOOP
	 
	pcap_breakloop(pd);
#else
	 
	if (pd != NULL && pcap_file(pd) == NULL) {
		 
		putchar('\n');
		(void)fflush(stdout);
		info(1);
	}
	exit_tcpdump(0);
#endif
}
