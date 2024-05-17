mm_record_login(Session *s, struct passwd *pw)
{
	socklen_t fromlen;
	struct sockaddr_storage from;

	if (options.use_login)
		return;

	 
	memset(&from, 0, sizeof(from));
	fromlen = sizeof(from);
	if (packet_connection_is_on_socket()) {
		if (getpeername(packet_get_connection_in(),
		    (struct sockaddr *)&from, &fromlen) < 0) {
			debug("getpeername: %.100s", strerror(errno));
			cleanup_exit(255);
		}
	}
	 
	record_login(s->pid, s->tty, pw->pw_name, pw->pw_uid,
	    get_remote_name_or_ip(utmp_len, options.use_dns),
	    (struct sockaddr *)&from, fromlen);
}
