 ssh_session(void)
 {
 	int type;
 	int interactive = 0;
 	int have_tty = 0;
  	struct winsize ws;
  	char *cp;
  	const char *display;
// 	char *proto = NULL, *data = NULL;
  
  	 
  	if (options.compression) {
 		    options.compression_level);
 
 		if (options.compression_level < 1 ||
 		    options.compression_level > 9)
 			fatal("Compression level must be from 1 (fast) to "
 			    "9 (slow, best).");
 
 		 
 		packet_start(SSH_CMSG_REQUEST_COMPRESSION);
 		packet_put_int(options.compression_level);
 		packet_send();
 		packet_write_wait();
 		type = packet_read();
 		if (type == SSH_SMSG_SUCCESS)
 			packet_start_compression(options.compression_level);
 		else if (type == SSH_SMSG_FAILURE)
 			logit("Warning: Remote host refused compression.");
 		else
 			packet_disconnect("Protocol error waiting for "
 			    "compression response.");
 	}
 	 
 	if (tty_flag) {
 		debug("Requesting pty.");
 
 		 
 		packet_start(SSH_CMSG_REQUEST_PTY);
 
 		 
 		cp = getenv("TERM");
 		if (!cp)
 			cp = "";
 		packet_put_cstring(cp);
 
 		 
 		if (ioctl(fileno(stdin), TIOCGWINSZ, &ws) < 0)
 			memset(&ws, 0, sizeof(ws));
 		packet_put_int((u_int)ws.ws_row);
 		packet_put_int((u_int)ws.ws_col);
 		packet_put_int((u_int)ws.ws_xpixel);
 		packet_put_int((u_int)ws.ws_ypixel);
 
 		 
 		tty_make_modes(fileno(stdin), NULL);
 
 		 
 		packet_send();
 		packet_write_wait();
 
 		 
 		type = packet_read();
 		if (type == SSH_SMSG_SUCCESS) {
 			interactive = 1;
 			have_tty = 1;
 		} else if (type == SSH_SMSG_FAILURE)
 			logit("Warning: Remote host failed or refused to "
 			    "allocate a pseudo tty.");
 		else
 			packet_disconnect("Protocol error waiting for pty "
 			    "request response.");
 	}
 	 
 	display = getenv("DISPLAY");
  	display = getenv("DISPLAY");
  	if (display == NULL && options.forward_x11)
  		debug("X11 forwarding requested but DISPLAY not set");
	if (options.forward_x11 && display != NULL) {
		char *proto, *data;
		 
		client_x11_get_proto(display, options.xauth_location,
		    options.forward_x11_trusted,
		    options.forward_x11_timeout,
		    &proto, &data);
// 	if (options.forward_x11 && client_x11_get_proto(display,
// 	    options.xauth_location, options.forward_x11_trusted,
// 	    options.forward_x11_timeout, &proto, &data) == 0) {
  		 
  		debug("Requesting X11 forwarding with authentication "
  		    "spoofing.");
 		 
 		debug("Requesting X11 forwarding with authentication "
 		    "spoofing.");
 		x11_request_forwarding_with_spoofing(0, display, proto,
 		    data, 0);
 		 
 		type = packet_read();
 		if (type == SSH_SMSG_SUCCESS) {
 			interactive = 1;
 		} else if (type == SSH_SMSG_FAILURE) {
 			logit("Warning: Remote host denied X11 forwarding.");
 		} else {
 			packet_disconnect("Protocol error waiting for X11 "
 			    "forwarding");
 		}
 	}
 	 
 	packet_set_interactive(interactive,
 	    options.ip_qos_interactive, options.ip_qos_bulk);
 
 	 
 	check_agent_present();
 
 	if (options.forward_agent) {
 		debug("Requesting authentication agent forwarding.");
 		auth_request_forwarding();
 
 		 
 		type = packet_read();
 		packet_check_eom();
 		if (type != SSH_SMSG_SUCCESS)
 			logit("Warning: Remote host denied authentication agent forwarding.");
 	}
 
 	 
 	ssh_init_stdio_forwarding();
 	ssh_init_forwarding();
 
 	 
 	if (options.local_command != NULL &&
 	    options.permit_local_command)
 		ssh_local_cmd(options.local_command);
 
 	 
 	if (fork_after_authentication_flag) {
 		if (options.exit_on_forward_failure &&
 		    options.num_remote_forwards > 0) {
 			debug("deferring postauth fork until remote forward "
 			    "confirmation received");
 		} else
 			fork_postauth();
 	}
 
 	 
 	if (buffer_len(&command) > 0) {
 		int len = buffer_len(&command);
 		if (len > 900)
 			len = 900;
 		debug("Sending command: %.*s", len,
 		    (u_char *)buffer_ptr(&command));
 		packet_start(SSH_CMSG_EXEC_CMD);
 		packet_put_string(buffer_ptr(&command), buffer_len(&command));
 		packet_send();
 		packet_write_wait();
 	} else {
 		debug("Requesting shell.");
 		packet_start(SSH_CMSG_EXEC_SHELL);
 		packet_send();
 		packet_write_wait();
 	}
 
 	 
 	return client_loop(have_tty, tty_flag ?
 	    options.escape_char : SSH_ESCAPECHAR_NONE, 0);
 }