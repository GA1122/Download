do_exec_no_pty(Session *s, const char *command)
{
	pid_t pid;

#ifdef USE_PIPES
	int pin[2], pout[2], perr[2];

	if (s == NULL)
		fatal("do_exec_no_pty: no session");

	 
	if (pipe(pin) < 0) {
		error("%s: pipe in: %.100s", __func__, strerror(errno));
		return -1;
	}
	if (pipe(pout) < 0) {
		error("%s: pipe out: %.100s", __func__, strerror(errno));
		close(pin[0]);
		close(pin[1]);
		return -1;
	}
	if (pipe(perr) < 0) {
		error("%s: pipe err: %.100s", __func__,
		    strerror(errno));
		close(pin[0]);
		close(pin[1]);
		close(pout[0]);
		close(pout[1]);
		return -1;
	}
#else
	int inout[2], err[2];

	if (s == NULL)
		fatal("do_exec_no_pty: no session");

	 
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, inout) < 0) {
		error("%s: socketpair #1: %.100s", __func__, strerror(errno));
		return -1;
	}
	if (socketpair(AF_UNIX, SOCK_STREAM, 0, err) < 0) {
		error("%s: socketpair #2: %.100s", __func__,
		    strerror(errno));
		close(inout[0]);
		close(inout[1]);
		return -1;
	}
#endif

	session_proctitle(s);

	 
	switch ((pid = fork())) {
	case -1:
		error("%s: fork: %.100s", __func__, strerror(errno));
#ifdef USE_PIPES
		close(pin[0]);
		close(pin[1]);
		close(pout[0]);
		close(pout[1]);
		close(perr[0]);
		close(perr[1]);
#else
		close(inout[0]);
		close(inout[1]);
		close(err[0]);
		close(err[1]);
#endif
		return -1;
	case 0:
		is_child = 1;

		 
		log_init(__progname, options.log_level,
		    options.log_facility, log_stderr);

		 
		if (setsid() < 0)
			error("setsid failed: %.100s", strerror(errno));

#ifdef USE_PIPES
		 
		close(pin[1]);
		if (dup2(pin[0], 0) < 0)
			perror("dup2 stdin");
		close(pin[0]);

		 
		close(pout[0]);
		if (dup2(pout[1], 1) < 0)
			perror("dup2 stdout");
		close(pout[1]);

		 
		close(perr[0]);
		if (dup2(perr[1], 2) < 0)
			perror("dup2 stderr");
		close(perr[1]);
#else
		 
		close(inout[1]);
		close(err[1]);
		if (dup2(inout[0], 0) < 0)	 
			perror("dup2 stdin");
		if (dup2(inout[0], 1) < 0)	 
			perror("dup2 stdout");
		close(inout[0]);
		if (dup2(err[0], 2) < 0)	 
			perror("dup2 stderr");
		close(err[0]);
#endif


#ifdef _UNICOS
		cray_init_job(s->pw);  
#endif

		 
		do_child(s, command);
		 
	default:
		break;
	}

#ifdef _UNICOS
	signal(WJSIGNAL, cray_job_termination_handler);
#endif  
#ifdef HAVE_CYGWIN
	cygwin_set_impersonation_token(INVALID_HANDLE_VALUE);
#endif

	s->pid = pid;
	 
	packet_set_interactive(s->display != NULL,
	    options.ip_qos_interactive, options.ip_qos_bulk);

	 
	buffer_clear(&loginmsg);

#ifdef USE_PIPES
	 
	close(pin[0]);
	close(pout[1]);
	close(perr[1]);

	if (compat20) {
		session_set_fds(s, pin[1], pout[0], perr[0],
		    s->is_subsystem, 0);
	} else {
		 
		server_loop(pid, pin[1], pout[0], perr[0]);
		 
	}
#else
	 
	close(inout[0]);
	close(err[0]);

	 
	if (compat20) {
		session_set_fds(s, inout[1], inout[1], err[1],
		    s->is_subsystem, 0);
	} else {
		server_loop(pid, inout[1], inout[1], err[1]);
		 
	}
#endif
	return 0;
}
