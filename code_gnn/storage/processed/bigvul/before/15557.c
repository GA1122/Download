process_mux_stop_listening(u_int rid, Channel *c, Buffer *m, Buffer *r)
{
	debug("%s: channel %d: stop listening", __func__, c->self);

	if (options.control_master == SSHCTL_MASTER_ASK ||
	    options.control_master == SSHCTL_MASTER_AUTO_ASK) {
		if (!ask_permission("Disable further multiplexing on shared "
		    "connection to %s? ", host)) {
			debug2("%s: stop listen refused by user", __func__);
			buffer_put_int(r, MUX_S_PERMISSION_DENIED);
			buffer_put_int(r, rid);
			buffer_put_cstring(r, "Permission denied");
			return 0;
		}
	}

	if (mux_listener_channel != NULL) {
		channel_free(mux_listener_channel);
		client_stop_mux();
		free(options.control_path);
		options.control_path = NULL;
		mux_listener_channel = NULL;
		muxserver_sock = -1;
	}

	 
	buffer_put_int(r, MUX_S_OK);
	buffer_put_int(r, rid);

	return 0;
}
