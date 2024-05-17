mux_client_request_stdio_fwd(int fd)
{
	Buffer m;
	char *e;
	u_int type, rid, sid;
	int devnull;

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

	buffer_init(&m);
	buffer_put_int(&m, MUX_C_NEW_STDIO_FWD);
	buffer_put_int(&m, muxclient_request_id);
	buffer_put_cstring(&m, "");  
	buffer_put_cstring(&m, stdio_forward_host);
	buffer_put_int(&m, stdio_forward_port);

	if (mux_client_write_packet(fd, &m) != 0)
		fatal("%s: write packet: %s", __func__, strerror(errno));

	 
	if (mm_send_fd(fd, STDIN_FILENO) == -1 ||
	    mm_send_fd(fd, STDOUT_FILENO) == -1)
		fatal("%s: send fds failed", __func__);

	if (pledge("stdio proc tty", NULL) == -1)
		fatal("%s pledge(): %s", __func__, strerror(errno));
	platform_pledge_mux();

	debug3("%s: stdio forward request sent", __func__);

	 
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
		fatal("Master refused stdio forwarding request: %s", e);
	case MUX_S_FAILURE:
		e = buffer_get_string(&m, NULL);
		buffer_free(&m);
		fatal("Stdio forwarding request failed: %s", e);
	default:
		buffer_free(&m);
		error("%s: unexpected response from master 0x%08x",
		    __func__, type);
		return -1;
	}
	muxclient_request_id++;

	signal(SIGHUP, control_client_sighandler);
	signal(SIGINT, control_client_sighandler);
	signal(SIGTERM, control_client_sighandler);
	signal(SIGWINCH, control_client_sigrelay);

	 
	buffer_clear(&m);
	if (mux_client_read_packet(fd, &m) != 0) {
		if (errno == EPIPE ||
		    (errno == EINTR && muxclient_terminate != 0))
			return 0;
		fatal("%s: mux_client_read_packet: %s",
		    __func__, strerror(errno));
	}
	fatal("%s: master returned unexpected message %u", __func__, type);
}
