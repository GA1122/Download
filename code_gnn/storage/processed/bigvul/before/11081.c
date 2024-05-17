static NTSTATUS smbd_server_connection_loop_once(struct smbd_server_connection *conn)
{
	fd_set r_fds, w_fds;
	int selrtn;
	struct timeval to;
	int maxfd = 0;

	to.tv_sec = SMBD_SELECT_TIMEOUT;
	to.tv_usec = 0;

	 

	FD_ZERO(&r_fds);
	FD_ZERO(&w_fds);

	 

	{
		struct timeval now;
		GetTimeOfDay(&now);

		event_add_to_select_args(smbd_event_context(), &now,
					 &r_fds, &w_fds, &to, &maxfd);
	}

	 
	if (run_events(smbd_event_context(), 0, NULL, NULL)) {
		return NT_STATUS_RETRY;
	}

	{
		int sav;
		START_PROFILE(smbd_idle);

		selrtn = sys_select(maxfd+1,&r_fds,&w_fds,NULL,&to);
		sav = errno;

		END_PROFILE(smbd_idle);
		errno = sav;
	}

	if (run_events(smbd_event_context(), selrtn, &r_fds, &w_fds)) {
		return NT_STATUS_RETRY;
	}

	 
	if (selrtn == -1) {
		 
		return map_nt_error_from_unix(errno);
	}

	 
	if (selrtn == 0) {
		return NT_STATUS_RETRY;
	}

	 
	return NT_STATUS_INTERNAL_ERROR;
}
