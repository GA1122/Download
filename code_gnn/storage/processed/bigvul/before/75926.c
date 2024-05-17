connection_in_progress(thread_t * thread)
{
	int status;

	DBG("SMTP connection to %s now IN_PROGRESS.",
	    FMT_SMTP_HOST());

	 
	status = tcp_socket_state(thread, connection_in_progress);

	if (status != connect_in_progress)
		SMTP_FSM_SEND(status, thread);

	return 0;
}