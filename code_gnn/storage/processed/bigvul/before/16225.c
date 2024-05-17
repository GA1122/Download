GahpClient::reset_user_timer(int tid)
{
	int retval = TRUE;

	if ( user_timerid != -1 ) {
		retval =  daemonCore->Reset_Timer(user_timerid,0);
	}
	
	if ( pending_timeout_tid != -1 ) {
		if ( tid < 0 ) {
			daemonCore->Cancel_Timer(pending_timeout_tid);
		}
		pending_timeout_tid = -1;
	}

	return retval;
}
