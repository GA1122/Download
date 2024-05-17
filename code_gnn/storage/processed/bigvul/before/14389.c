do_exec_pty(Session *s, const char *command)
{
	int fdout, ptyfd, ttyfd, ptymaster;
	pid_t pid;

	if (s == NULL)
		fatal("do_exec_pty: no session");
	ptyfd = s->ptyfd;
	ttyfd = s->ttyfd;

	 
	if ((fdout = dup(ptyfd)) < 0) {
		error("%s: dup #1: %s", __func__, strerror(errno));
		close(ttyfd);
		close(ptyfd);
		return -1;
	}
	 
	if ((ptymaster = dup(ptyfd)) < 0) {
		error("%s: dup #2: %s", __func__, strerror(errno));
		close(ttyfd);
		close(ptyfd);
		close(fdout);
		return -1;
	}

	 
	switch ((pid = fork())) {
	case -1:
		error("%s: fork: %.100s", __func__, strerror(errno));
		close(fdout);
		close(ptymaster);
		close(ttyfd);
		close(ptyfd);
		return -1;
	case 0:
		is_child = 1;

		close(fdout);
		close(ptymaster);

		 
		log_init(__progname, options.log_level,
		    options.log_facility, log_stderr);
		 
		close(ptyfd);

		 
		pty_make_controlling_tty(&ttyfd, s->tty);

		 
		if (dup2(ttyfd, 0) < 0)
			error("dup2 stdin: %s", strerror(errno));
		if (dup2(ttyfd, 1) < 0)
			error("dup2 stdout: %s", strerror(errno));
		if (dup2(ttyfd, 2) < 0)
			error("dup2 stderr: %s", strerror(errno));

		 
		close(ttyfd);

		 
#ifndef HAVE_OSF_SIA
		if (!(options.use_login && command == NULL)) {
#ifdef _UNICOS
			cray_init_job(s->pw);  
#endif  
			do_login(s, command);
		}
# ifdef LOGIN_NEEDS_UTMPX
		else
			do_pre_login(s);
# endif
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

	 
	close(ttyfd);

	 
	s->ptymaster = ptymaster;
	packet_set_interactive(1, 
	    options.ip_qos_interactive, options.ip_qos_bulk);
	if (compat20) {
		session_set_fds(s, ptyfd, fdout, -1, 1, 1);
	} else {
		server_loop(pid, ptyfd, fdout, -1);
		 
	}
	return 0;
}
