muxclient(const char *path)
{
	struct sockaddr_un addr;
	socklen_t sun_len;
	int sock;
	u_int pid;

	if (muxclient_command == 0) {
		if (stdio_forward_host != NULL)
			muxclient_command = SSHMUX_COMMAND_STDIO_FWD;
		else
			muxclient_command = SSHMUX_COMMAND_OPEN;
	}

	switch (options.control_master) {
	case SSHCTL_MASTER_AUTO:
	case SSHCTL_MASTER_AUTO_ASK:
		debug("auto-mux: Trying existing master");
		 
	case SSHCTL_MASTER_NO:
		break;
	default:
		return;
	}

	memset(&addr, '\0', sizeof(addr));
	addr.sun_family = AF_UNIX;
	sun_len = offsetof(struct sockaddr_un, sun_path) +
	    strlen(path) + 1;

	if (strlcpy(addr.sun_path, path,
	    sizeof(addr.sun_path)) >= sizeof(addr.sun_path))
		fatal("ControlPath too long");

	if ((sock = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)
		fatal("%s socket(): %s", __func__, strerror(errno));

	if (connect(sock, (struct sockaddr *)&addr, sun_len) == -1) {
		switch (muxclient_command) {
		case SSHMUX_COMMAND_OPEN:
		case SSHMUX_COMMAND_STDIO_FWD:
			break;
		default:
			fatal("Control socket connect(%.100s): %s", path,
			    strerror(errno));
		}
		if (errno == ECONNREFUSED &&
		    options.control_master != SSHCTL_MASTER_NO) {
			debug("Stale control socket %.100s, unlinking", path);
			unlink(path);
		} else if (errno == ENOENT) {
			debug("Control socket \"%.100s\" does not exist", path);
		} else {
			error("Control socket connect(%.100s): %s", path,
			    strerror(errno));
		}
		close(sock);
		return;
	}
	set_nonblock(sock);

	if (mux_client_hello_exchange(sock) != 0) {
		error("%s: master hello exchange failed", __func__);
		close(sock);
		return;
	}

	switch (muxclient_command) {
	case SSHMUX_COMMAND_ALIVE_CHECK:
		if ((pid = mux_client_request_alive(sock)) == 0)
			fatal("%s: master alive check failed", __func__);
		fprintf(stderr, "Master running (pid=%u)\r\n", pid);
		exit(0);
	case SSHMUX_COMMAND_TERMINATE:
		mux_client_request_terminate(sock);
		fprintf(stderr, "Exit request sent.\r\n");
		exit(0);
	case SSHMUX_COMMAND_FORWARD:
		if (mux_client_forwards(sock, 0) != 0)
			fatal("%s: master forward request failed", __func__);
		exit(0);
	case SSHMUX_COMMAND_OPEN:
		if (mux_client_forwards(sock, 0) != 0) {
			error("%s: master forward request failed", __func__);
			return;
		}
		mux_client_request_session(sock);
		return;
	case SSHMUX_COMMAND_STDIO_FWD:
		mux_client_request_stdio_fwd(sock);
		exit(0);
	case SSHMUX_COMMAND_STOP:
		mux_client_request_stop_listening(sock);
		fprintf(stderr, "Stop listening request sent.\r\n");
		exit(0);
	case SSHMUX_COMMAND_CANCEL_FWD:
		if (mux_client_forwards(sock, 1) != 0)
			error("%s: master cancel forward request failed",
			    __func__);
		exit(0);
	default:
		fatal("unrecognised muxclient_command %d", muxclient_command);
	}
}
