do_authenticated1(Authctxt *authctxt)
{
	Session *s;
	char *command;
	int success, type, screen_flag;
	int enable_compression_after_reply = 0;
	u_int proto_len, data_len, dlen, compression_level = 0;

	s = session_new();
	if (s == NULL) {
		error("no more sessions");
		return;
	}
	s->authctxt = authctxt;
	s->pw = authctxt->pw;

	 
	for (;;) {
		success = 0;

		 
		type = packet_read();

		 
		switch (type) {
		case SSH_CMSG_REQUEST_COMPRESSION:
			compression_level = packet_get_int();
			packet_check_eom();
			if (compression_level < 1 || compression_level > 9) {
				packet_send_debug("Received invalid compression level %d.",
				    compression_level);
				break;
			}
			if (options.compression == COMP_NONE) {
				debug2("compression disabled");
				break;
			}
			 
			enable_compression_after_reply = 1;
			success = 1;
			break;

		case SSH_CMSG_REQUEST_PTY:
			success = session_pty_req(s);
			break;

		case SSH_CMSG_X11_REQUEST_FORWARDING:
			s->auth_proto = packet_get_string(&proto_len);
			s->auth_data = packet_get_string(&data_len);

			screen_flag = packet_get_protocol_flags() &
			    SSH_PROTOFLAG_SCREEN_NUMBER;
			debug2("SSH_PROTOFLAG_SCREEN_NUMBER: %d", screen_flag);

			if (packet_remaining() == 4) {
				if (!screen_flag)
					debug2("Buggy client: "
					    "X11 screen flag missing");
				s->screen = packet_get_int();
			} else {
				s->screen = 0;
			}
			packet_check_eom();
			if (xauth_valid_string(s->auth_proto) &&
			    xauth_valid_string(s->auth_data))
				success = session_setup_x11fwd(s);
			else {
				success = 0;
				error("Invalid X11 forwarding data");
			}
			if (!success) {
				free(s->auth_proto);
				free(s->auth_data);
				s->auth_proto = NULL;
				s->auth_data = NULL;
			}
			break;

		case SSH_CMSG_AGENT_REQUEST_FORWARDING:
			if (!options.allow_agent_forwarding ||
			    no_agent_forwarding_flag || compat13) {
				debug("Authentication agent forwarding not permitted for this authentication.");
				break;
			}
			debug("Received authentication agent forwarding request.");
			success = auth_input_request_forwarding(s->pw);
			break;

		case SSH_CMSG_PORT_FORWARD_REQUEST:
			if (no_port_forwarding_flag) {
				debug("Port forwarding not permitted for this authentication.");
				break;
			}
			if (!(options.allow_tcp_forwarding & FORWARD_REMOTE)) {
				debug("Port forwarding not permitted.");
				break;
			}
			debug("Received TCP/IP port forwarding request.");
			if (channel_input_port_forward_request(s->pw->pw_uid == 0,
			    &options.fwd_opts) < 0) {
				debug("Port forwarding failed.");
				break;
			}
			success = 1;
			break;

		case SSH_CMSG_MAX_PACKET_SIZE:
			if (packet_set_maxsize(packet_get_int()) > 0)
				success = 1;
			break;

		case SSH_CMSG_EXEC_SHELL:
		case SSH_CMSG_EXEC_CMD:
			if (type == SSH_CMSG_EXEC_CMD) {
				command = packet_get_string(&dlen);
				debug("Exec command '%.500s'", command);
				if (do_exec(s, command) != 0)
					packet_disconnect(
					    "command execution failed");
				free(command);
			} else {
				if (do_exec(s, NULL) != 0)
					packet_disconnect(
					    "shell execution failed");
			}
			packet_check_eom();
			session_close(s);
			return;

		default:
			 
			logit("Unknown packet type received after authentication: %d", type);
		}
		packet_start(success ? SSH_SMSG_SUCCESS : SSH_SMSG_FAILURE);
		packet_send();
		packet_write_wait();

		 
		if (enable_compression_after_reply) {
			enable_compression_after_reply = 0;
			packet_start_compression(compression_level);
		}
	}
}
