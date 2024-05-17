GahpServer::setPollInterval(unsigned int interval)
{
	if (poll_tid != -1) {
		daemonCore->Cancel_Timer(poll_tid);
		poll_tid = -1;
	}
	m_pollInterval = interval;
	if ( m_pollInterval > 0 ) {
		poll_tid = daemonCore->Register_Timer(m_pollInterval,
											m_pollInterval,
											(TimerHandlercpp)&GahpServer::poll,
											"GahpServer::poll",this);
	}
}
