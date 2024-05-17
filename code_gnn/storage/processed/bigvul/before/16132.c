GahpClient::check_pending_timeout(const char *,const char *)
{

	if ( pending_reqid == 0 ) {
		return false;
	}

	if ( pending_submitted_to_gahp == false ) {
		return false;
	}

	if ( pending_timeout && (time(NULL) > pending_timeout) ) {
		clear_pending();	 
		return true;
	}

	return false;
}
