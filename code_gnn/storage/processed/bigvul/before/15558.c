process_mux_terminate(u_int rid, Channel *c, Buffer *m, Buffer *r)
{
	debug2("%s: channel %d: terminate request", __func__, c->self);

	if (options.control_master == SSHCTL_MASTER_ASK ||
	    options.control_master == SSHCTL_MASTER_AUTO_ASK) {
		if (!ask_permission("Terminate shared connection to %s? ",
		    host)) {
			debug2("%s: termination refused by user", __func__);
			buffer_put_int(r, MUX_S_PERMISSION_DENIED);
			buffer_put_int(r, rid);
			buffer_put_cstring(r, "Permission denied");
			return 0;
		}
	}

	quit_pending = 1;
	buffer_put_int(r, MUX_S_OK);
	buffer_put_int(r, rid);
	 
	return 0;
}
