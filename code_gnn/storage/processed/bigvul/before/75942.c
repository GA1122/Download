smtp_connect(smtp_t * smtp)
{
	enum connect_result status;

	if ((smtp->fd = socket(global_data->smtp_server.ss_family, SOCK_STREAM | SOCK_CLOEXEC | SOCK_NONBLOCK, IPPROTO_TCP)) == -1) {
		DBG("SMTP connect fail to create socket.");
		free_smtp_all(smtp);
		return;
	}

#if !HAVE_DECL_SOCK_NONBLOCK
	if (set_sock_flags(smtp->fd, F_SETFL, O_NONBLOCK))
		log_message(LOG_INFO, "Unable to set NONBLOCK on smtp_connect socket - %s (%d)", strerror(errno), errno);
#endif

#if !HAVE_DECL_SOCK_CLOEXEC
	if (set_sock_flags(smtp->fd, F_SETFD, FD_CLOEXEC))
		log_message(LOG_INFO, "Unable to set CLOEXEC on smtp_connect socket - %s (%d)", strerror(errno), errno);
#endif

	status = tcp_connect(smtp->fd, &global_data->smtp_server);

	 
	thread_add_event(master, SMTP_FSM[status].send, smtp, smtp->fd);
}