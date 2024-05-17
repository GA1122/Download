do_login(Session *s, const char *command)
{
	struct ssh *ssh = active_state;	 
	socklen_t fromlen;
	struct sockaddr_storage from;
	struct passwd * pw = s->pw;
	pid_t pid = getpid();

	 
	memset(&from, 0, sizeof(from));
	fromlen = sizeof(from);
	if (packet_connection_is_on_socket()) {
		if (getpeername(packet_get_connection_in(),
		    (struct sockaddr *)&from, &fromlen) < 0) {
			debug("getpeername: %.100s", strerror(errno));
			cleanup_exit(255);
		}
	}

	 
	if (!use_privsep)
		record_login(pid, s->tty, pw->pw_name, pw->pw_uid,
		    session_get_remote_name_or_ip(ssh, utmp_len,
		    options.use_dns),
		    (struct sockaddr *)&from, fromlen);

#ifdef USE_PAM
	 
	if (options.use_pam && !use_privsep && s->authctxt->force_pwchange) {
		display_loginmsg();
		do_pam_chauthtok();
		s->authctxt->force_pwchange = 0;
		 
	}
#endif

	if (check_quietlogin(s, command))
		return;

	display_loginmsg();

	do_motd();
}
