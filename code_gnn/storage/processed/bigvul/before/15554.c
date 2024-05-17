process_mux_new_session(u_int rid, Channel *c, Buffer *m, Buffer *r)
{
	Channel *nc;
	struct mux_session_confirm_ctx *cctx;
	char *reserved, *cmd, *cp;
	u_int i, j, len, env_len, escape_char, window, packetmax;
	int new_fd[3];

	 
	cctx = xcalloc(1, sizeof(*cctx));
	cctx->term = NULL;
	cctx->rid = rid;
	cmd = reserved = NULL;
	cctx->env = NULL;
	env_len = 0;
	if ((reserved = buffer_get_string_ret(m, NULL)) == NULL ||
	    buffer_get_int_ret(&cctx->want_tty, m) != 0 ||
	    buffer_get_int_ret(&cctx->want_x_fwd, m) != 0 ||
	    buffer_get_int_ret(&cctx->want_agent_fwd, m) != 0 ||
	    buffer_get_int_ret(&cctx->want_subsys, m) != 0 ||
	    buffer_get_int_ret(&escape_char, m) != 0 ||
	    (cctx->term = buffer_get_string_ret(m, &len)) == NULL ||
	    (cmd = buffer_get_string_ret(m, &len)) == NULL) {
 malf:
		free(cmd);
		free(reserved);
		for (j = 0; j < env_len; j++)
			free(cctx->env[j]);
		free(cctx->env);
		free(cctx->term);
		free(cctx);
		error("%s: malformed message", __func__);
		return -1;
	}
	free(reserved);
	reserved = NULL;

	while (buffer_len(m) > 0) {
#define MUX_MAX_ENV_VARS	4096
		if ((cp = buffer_get_string_ret(m, &len)) == NULL)
			goto malf;
		if (!env_permitted(cp)) {
			free(cp);
			continue;
		}
		cctx->env = xreallocarray(cctx->env, env_len + 2,
		    sizeof(*cctx->env));
		cctx->env[env_len++] = cp;
		cctx->env[env_len] = NULL;
		if (env_len > MUX_MAX_ENV_VARS) {
			error(">%d environment variables received, ignoring "
			    "additional", MUX_MAX_ENV_VARS);
			break;
		}
	}

	debug2("%s: channel %d: request tty %d, X %d, agent %d, subsys %d, "
	    "term \"%s\", cmd \"%s\", env %u", __func__, c->self,
	    cctx->want_tty, cctx->want_x_fwd, cctx->want_agent_fwd,
	    cctx->want_subsys, cctx->term, cmd, env_len);

	buffer_init(&cctx->cmd);
	buffer_append(&cctx->cmd, cmd, strlen(cmd));
	free(cmd);
	cmd = NULL;

	 
	for(i = 0; i < 3; i++) {
		if ((new_fd[i] = mm_receive_fd(c->sock)) == -1) {
			error("%s: failed to receive fd %d from slave",
			    __func__, i);
			for (j = 0; j < i; j++)
				close(new_fd[j]);
			for (j = 0; j < env_len; j++)
				free(cctx->env[j]);
			free(cctx->env);
			free(cctx->term);
			buffer_free(&cctx->cmd);
			free(cctx);

			 
			buffer_put_int(r, MUX_S_FAILURE);
			buffer_put_int(r, rid);
			buffer_put_cstring(r,
			    "did not receive file descriptors");
			return -1;
		}
	}

	debug3("%s: got fds stdin %d, stdout %d, stderr %d", __func__,
	    new_fd[0], new_fd[1], new_fd[2]);

	 
	if (c->remote_id != -1) {
		debug2("%s: session already open", __func__);
		 
		buffer_put_int(r, MUX_S_FAILURE);
		buffer_put_int(r, rid);
		buffer_put_cstring(r, "Multiple sessions not supported");
 cleanup:
		close(new_fd[0]);
		close(new_fd[1]);
		close(new_fd[2]);
		free(cctx->term);
		if (env_len != 0) {
			for (i = 0; i < env_len; i++)
				free(cctx->env[i]);
			free(cctx->env);
		}
		buffer_free(&cctx->cmd);
		free(cctx);
		return 0;
	}

	if (options.control_master == SSHCTL_MASTER_ASK ||
	    options.control_master == SSHCTL_MASTER_AUTO_ASK) {
		if (!ask_permission("Allow shared connection to %s? ", host)) {
			debug2("%s: session refused by user", __func__);
			 
			buffer_put_int(r, MUX_S_PERMISSION_DENIED);
			buffer_put_int(r, rid);
			buffer_put_cstring(r, "Permission denied");
			goto cleanup;
		}
	}

	 
	if (cctx->want_tty && tcgetattr(new_fd[0], &cctx->tio) == -1)
		error("%s: tcgetattr: %s", __func__, strerror(errno));

	 
	if (!isatty(new_fd[0]))
		set_nonblock(new_fd[0]);
	if (!isatty(new_fd[1]))
		set_nonblock(new_fd[1]);
	if (!isatty(new_fd[2]))
		set_nonblock(new_fd[2]);

	window = CHAN_SES_WINDOW_DEFAULT;
	packetmax = CHAN_SES_PACKET_DEFAULT;
	if (cctx->want_tty) {
		window >>= 1;
		packetmax >>= 1;
	}

	nc = channel_new("session", SSH_CHANNEL_OPENING,
	    new_fd[0], new_fd[1], new_fd[2], window, packetmax,
	    CHAN_EXTENDED_WRITE, "client-session",  0);

	nc->ctl_chan = c->self;		 
	c->remote_id = nc->self; 	 

	if (cctx->want_tty && escape_char != 0xffffffff) {
		channel_register_filter(nc->self,
		    client_simple_escape_filter, NULL,
		    client_filter_cleanup,
		    client_new_escape_filter_ctx((int)escape_char));
	}

	debug2("%s: channel_new: %d linked to control channel %d",
	    __func__, nc->self, nc->ctl_chan);

	channel_send_open(nc->self);
	channel_register_open_confirm(nc->self, mux_session_confirm, cctx);
	c->mux_pause = 1;  
	channel_register_cleanup(nc->self, mux_master_session_cleanup_cb, 1);

	 
	return 0;
}
