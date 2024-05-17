process_mux_open_fwd(u_int rid, Channel *c, Buffer *m, Buffer *r)
{
	struct Forward fwd;
	char *fwd_desc = NULL;
	char *listen_addr, *connect_addr;
	u_int ftype;
	u_int lport, cport;
	int i, ret = 0, freefwd = 1;

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

	debug2("%s: channel %d: request %s", __func__, c->self,
	    (fwd_desc = format_forward(ftype, &fwd)));

	if (ftype != MUX_FWD_LOCAL && ftype != MUX_FWD_REMOTE &&
	    ftype != MUX_FWD_DYNAMIC) {
		logit("%s: invalid forwarding type %u", __func__, ftype);
 invalid:
		free(listen_addr);
		free(connect_addr);
		buffer_put_int(r, MUX_S_FAILURE);
		buffer_put_int(r, rid);
		buffer_put_cstring(r, "Invalid forwarding request");
		return 0;
	}
	if (ftype == MUX_FWD_DYNAMIC && fwd.listen_path) {
		logit("%s: streamlocal and dynamic forwards "
		    "are mutually exclusive", __func__);
		goto invalid;
	}
	if (fwd.listen_port != PORT_STREAMLOCAL && fwd.listen_port >= 65536) {
		logit("%s: invalid listen port %u", __func__,
		    fwd.listen_port);
		goto invalid;
	}
	if ((fwd.connect_port != PORT_STREAMLOCAL && fwd.connect_port >= 65536)
	    || (ftype != MUX_FWD_DYNAMIC && ftype != MUX_FWD_REMOTE && fwd.connect_port == 0)) {
		logit("%s: invalid connect port %u", __func__,
		    fwd.connect_port);
		goto invalid;
	}
	if (ftype != MUX_FWD_DYNAMIC && fwd.connect_host == NULL && fwd.connect_path == NULL) {
		logit("%s: missing connect host", __func__);
		goto invalid;
	}

	 
	switch (ftype) {
	case MUX_FWD_LOCAL:
	case MUX_FWD_DYNAMIC:
		for (i = 0; i < options.num_local_forwards; i++) {
			if (compare_forward(&fwd,
			    options.local_forwards + i)) {
 exists:
				debug2("%s: found existing forwarding",
				    __func__);
				buffer_put_int(r, MUX_S_OK);
				buffer_put_int(r, rid);
				goto out;
			}
		}
		break;
	case MUX_FWD_REMOTE:
		for (i = 0; i < options.num_remote_forwards; i++) {
			if (compare_forward(&fwd,
			    options.remote_forwards + i)) {
				if (fwd.listen_port != 0)
					goto exists;
				debug2("%s: found allocated port",
				    __func__);
				buffer_put_int(r, MUX_S_REMOTE_PORT);
				buffer_put_int(r, rid);
				buffer_put_int(r,
				    options.remote_forwards[i].allocated_port);
				goto out;
			}
		}
		break;
	}

	if (options.control_master == SSHCTL_MASTER_ASK ||
	    options.control_master == SSHCTL_MASTER_AUTO_ASK) {
		if (!ask_permission("Open %s on %s?", fwd_desc, host)) {
			debug2("%s: forwarding refused by user", __func__);
			buffer_put_int(r, MUX_S_PERMISSION_DENIED);
			buffer_put_int(r, rid);
			buffer_put_cstring(r, "Permission denied");
			goto out;
		}
	}

	if (ftype == MUX_FWD_LOCAL || ftype == MUX_FWD_DYNAMIC) {
		if (!channel_setup_local_fwd_listener(&fwd,
		    &options.fwd_opts)) {
 fail:
			logit("slave-requested %s failed", fwd_desc);
			buffer_put_int(r, MUX_S_FAILURE);
			buffer_put_int(r, rid);
			buffer_put_cstring(r, "Port forwarding failed");
			goto out;
		}
		add_local_forward(&options, &fwd);
		freefwd = 0;
	} else {
		struct mux_channel_confirm_ctx *fctx;

		fwd.handle = channel_request_remote_forwarding(&fwd);
		if (fwd.handle < 0)
			goto fail;
		add_remote_forward(&options, &fwd);
		fctx = xcalloc(1, sizeof(*fctx));
		fctx->cid = c->self;
		fctx->rid = rid;
		fctx->fid = options.num_remote_forwards - 1;
		client_register_global_confirm(mux_confirm_remote_forward,
		    fctx);
		freefwd = 0;
		c->mux_pause = 1;  
		 
		goto out;
	}
	buffer_put_int(r, MUX_S_OK);
	buffer_put_int(r, rid);
 out:
	free(fwd_desc);
	if (freefwd) {
		free(fwd.listen_host);
		free(fwd.listen_path);
		free(fwd.connect_host);
		free(fwd.connect_path);
	}
	return ret;
}
