GahpServer::ProxyCallback()
{
	if ( m_gahp_pid > 0 ) {
		proxy_check_tid = daemonCore->Register_Timer( 0,
								(TimerHandlercpp)&GahpServer::doProxyCheck,
								"GahpServer::doProxyCheck", (Service*)this );
	}
	return 0;
}
