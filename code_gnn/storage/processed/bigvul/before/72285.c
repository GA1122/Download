grace_alarm_handler(int sig)
{
	if (use_privsep && pmonitor != NULL && pmonitor->m_pid > 0)
		kill(pmonitor->m_pid, SIGALRM);

	 
	if (getpgid(0) == getpid()) {
		signal(SIGTERM, SIG_IGN);
		kill(0, SIGTERM);
	}

	 
	sigdie("Timeout before authentication for %s port %d",
	    ssh_remote_ipaddr(active_state), ssh_remote_port(active_state));
}
