GahpClient::get_pending_result(const char *,const char *)
{
	Gahp_Args* r = NULL;

	if ( (m_mode == blocking) && (!pending_result) ) {
		for (;;) {
			server->poll();
			if ( pending_result ) {
				break;
			}
			if ( pending_timeout && (time(NULL) > pending_timeout) ) {
				break;
			}
			sleep(1);	 
		}
	}

	if ( pending_result ) {
		ASSERT(pending_reqid == 0);
		r = pending_result;
		pending_result = NULL;   
		clear_pending();
	}

	return r;
}
