process_mux_close_fwd(u_int rid, Channel *c, Buffer *m, Buffer *r)
{
	struct Forward fwd, *found_fwd;
	char *fwd_desc = NULL;
	const char *error_reason = NULL;
	char *listen_addr = NULL, *connect_addr = NULL;
	u_int ftype;
	int i, ret = 0;
	u_int lport, cport;

	memset(&fwd, 0, sizeof(fwd));

	if (buffer_get_int_ret(&ftype, m) != 0 ||
	    (listen_addr = buffer_get_string_ret(m, NULL)) == NULL ||
	    buffer_get_int_ret(&lport, m) != 0 ||
	    (connect_addr = buffer_get_string_ret(m, NULL)) == NULL ||
	    buffer_get_int_ret(&cport, m) != 0 ||
	    (lport != (u_int)PORT_STREAMLOCAL && lport > 65535) ||
	    (cport != (u_int)PORT_STREAMLOCAL && cport > 65535)) {
		error("%s: malformed message", __func__);
		ret = -1;
		goto out;
	}

	if (*listen_addr == '\0') {
		free(listen_addr);
		listen_addr = NULL;
	}
	if (*connect_addr == '\0') {
		free(connect_addr);
		connect_addr = NULL;
	}

	memset(&fwd, 0, sizeof(fwd));
	fwd.listen_port = lport;
	if (fwd.listen_port == PORT_STREAMLOCAL)
		fwd.listen_path = listen_addr;
	else
		fwd.listen_host = listen_addr;
	fwd.connect_port = cport;
	if (fwd.connect_port == PORT_STREAMLOCAL)
		fwd.connect_path = connect_addr;
	else
		fwd.connect_host = connect_addr;

	debug2("%s: channel %d: request cancel %s", __func__, c->self,
	    (fwd_desc = format_forward(ftype, &fwd)));

	 
	found_fwd = NULL;
	switch (ftype) {
	case MUX_FWD_LOCAL:
	case MUX_FWD_DYNAMIC:
		for (i = 0; i < options.num_local_forwards; i++) {
			if (compare_forward(&fwd,
			    options.local_forwards + i)) {
				found_fwd = options.local_forwards + i;
				break;
			}
		}
		break;
	case MUX_FWD_REMOTE:
		for (i = 0; i < options.num_remote_forwards; i++) {
			if (compare_forward(&fwd,
			    options.remote_forwards + i)) {
				found_fwd = options.remote_forwards + i;
				break;
			}
		}
		break;
	}

	if (found_fwd == NULL)
		error_reason = "port not forwarded";
	else if (ftype == MUX_FWD_REMOTE) {
		 
		if (channel_request_rforward_cancel(found_fwd) == -1)
			error_reason = "port not in permitted opens";
	} else {	 
		 
		if (channel_cancel_lport_listener(&fwd, fwd.connect_port,
		    &options.fwd_opts) == -1)
			error_reason = "port not found";
	}

	if (error_reason == NULL) {
		buffer_put_int(r, MUX_S_OK);
		buffer_put_int(r, rid);

		free(found_fwd->listen_host);
		free(found_fwd->listen_path);
		free(found_fwd->connect_host);
		free(found_fwd->connect_path);
		found_fwd->listen_host = found_fwd->connect_host = NULL;
		found_fwd->listen_path = found_fwd->connect_path = NULL;
		found_fwd->listen_port = found_fwd->connect_port = 0;
	} else {
		buffer_put_int(r, MUX_S_FAILURE);
		buffer_put_int(r, rid);
		buffer_put_cstring(r, error_reason);
	}
 out:
	free(fwd_desc);
	free(listen_addr);
	free(connect_addr);

	return ret;
}
