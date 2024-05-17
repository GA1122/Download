mux_client_request_session(int fd)
{
	Buffer m;
	char *e, *term;
	u_int i, rid, sid, esid, exitval, type, exitval_seen;
	extern char **environ;
	int devnull, rawmode;

	debug3("%s: entering", __func__);

	if ((muxserver_pid = mux_client_request_alive(fd)) == 0) {
		error("%s: master alive request failed", __func__);
		return -1;
	}

	signal(SIGPIPE, SIG_IGN);

	if (stdin_null_flag) {
		if ((devnull = open(_PATH_DEVNULL, O_RDONLY)) == -1)
			fatal("open(/dev/null): %s", strerror(errno));
		if (dup2(devnull, STDIN_FILENO) == -1)
			fatal("dup2: %s", strerror(errno));
		if (devnull > STDERR_FILENO)
			close(devnull);
	}

	term = getenv("TERM");

	buffer_init(&m);
	buffer_put_int(&m, MUX_C_NEW_SESSION);
	buffer_put_int(&m, muxclient_request_id);
	buffer_put_cstring(&m, "");  
	buffer_put_int(&m, tty_flag);
	buffer_put_int(&m, options.forward_x11);
	buffer_put_int(&m, options.forward_agent);
	buffer_put_int(&m, subsystem_flag);
	buffer_put_int(&m, options.escape_char == SSH_ESCAPECHAR_NONE ?
	    0xffffffff : (u_int)options.escape_char);
	buffer_put_cstring(&m, term == NULL ? "" : term);
	buffer_put_string(&m, buffer_ptr(&command), buffer_len(&command));

	if (options.num_send_env > 0 && environ != NULL) {
		 
		for (i = 0; environ[i] != NULL; i++) {
			if (env_permitted(environ[i])) {
				buffer_put_cstring(&m, environ[i]);
			}
		}
	}

	if (mux_client_write_packet(fd, &m) != 0)
		fatal("%s: write packet: %s", __func__, strerror(errno));

	 
	if (mm_send_fd(fd, STDIN_FILENO) == -1 ||
	    mm_send_fd(fd, STDOUT_FILENO) == -1 ||
	    mm_send_fd(fd, STDERR_FILENO) == -1)
		fatal("%s: send fds failed", __func__);

	debug3("%s: session request sent", __func__);

	 
	buffer_clear(&m);
	if (mux_client_read_packet(fd, &m) != 0) {
		error("%s: read from master failed: %s",
		    __func__, strerror(errno));
		buffer_free(&m);
		return -1;
	}

	type = buffer_get_int(&m);
	if ((rid = buffer_get_int(&m)) != muxclient_request_id)
		fatal("%s: out of sequence reply: my id %u theirs %u",
		    __func__, muxclient_request_id, rid);
	switch (type) {
	case MUX_S_SESSION_OPENED:
		sid = buffer_get_int(&m);
		debug("%s: master session id: %u", __func__, sid);
		break;
	case MUX_S_PERMISSION_DENIED:
		e = buffer_get_string(&m, NULL);
		buffer_free(&m);
		error("Master refused session request: %s", e);
		return -1;
	case MUX_S_FAILURE:
		e = buffer_get_string(&m, NULL);
		buffer_free(&m);
		error("%s: session request failed: %s", __func__, e);
		return -1;
	default:
		buffer_free(&m);
		error("%s: unexpected response from master 0x%08x",
		    __func__, type);
		return -1;
	}
	muxclient_request_id++;

	if (pledge("stdio proc tty", NULL) == -1)
		fatal("%s pledge(): %s", __func__, strerror(errno));
	platform_pledge_mux();

	signal(SIGHUP, control_client_sighandler);
	signal(SIGINT, control_client_sighandler);
	signal(SIGTERM, control_client_sighandler);
	signal(SIGWINCH, control_client_sigrelay);

	rawmode = tty_flag;
	if (tty_flag)
		enter_raw_mode(options.request_tty == REQUEST_TTY_FORCE);

	 
	for (exitval = 255, exitval_seen = 0;;) {
		buffer_clear(&m);
		if (mux_client_read_packet(fd, &m) != 0)
			break;
		type = buffer_get_int(&m);
		switch (type) {
		case MUX_S_TTY_ALLOC_FAIL:
			if ((esid = buffer_get_int(&m)) != sid)
				fatal("%s: tty alloc fail on unknown session: "
				    "my id %u theirs %u",
				    __func__, sid, esid);
			leave_raw_mode(options.request_tty ==
			    REQUEST_TTY_FORCE);
			rawmode = 0;
			continue;
		case MUX_S_EXIT_MESSAGE:
			if ((esid = buffer_get_int(&m)) != sid)
				fatal("%s: exit on unknown session: "
				    "my id %u theirs %u",
				    __func__, sid, esid);
			if (exitval_seen)
				fatal("%s: exitval sent twice", __func__);
			exitval = buffer_get_int(&m);
			exitval_seen = 1;
			continue;
		default:
			e = buffer_get_string(&m, NULL);
			fatal("%s: master returned error: %s", __func__, e);
		}
	}

	close(fd);
	if (rawmode)
		leave_raw_mode(options.request_tty == REQUEST_TTY_FORCE);

	if (muxclient_terminate) {
		debug2("Exiting on signal %d", muxclient_terminate);
		exitval = 255;
	} else if (!exitval_seen) {
		debug2("Control master terminated unexpectedly");
		exitval = 255;
	} else
		debug2("Received exit status from master %d", exitval);

	if (tty_flag && options.log_level != SYSLOG_LEVEL_QUIET)
		fprintf(stderr, "Shared connection to %s closed.\r\n", host);

	exit(exitval);
}
