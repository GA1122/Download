GahpServer::poll_real_soon()
{
	if (!poll_pending) {
		int tid = daemonCore->Register_Timer(0,
			(TimerHandlercpp)&GahpServer::poll,
			"GahpServer::poll from poll_real_soon",this);
		if ( tid != -1 ) {
			poll_pending = true;
		}
	}
}
