process_mux_master_hello(u_int rid, Channel *c, Buffer *m, Buffer *r)
{
	u_int ver;
	struct mux_master_state *state = (struct mux_master_state *)c->mux_ctx;

	if (state == NULL)
		fatal("%s: channel %d: c->mux_ctx == NULL", __func__, c->self);
	if (state->hello_rcvd) {
		error("%s: HELLO received twice", __func__);
		return -1;
	}
	if (buffer_get_int_ret(&ver, m) != 0) {
 malf:
		error("%s: malformed message", __func__);
		return -1;
	}
	if (ver != SSHMUX_VER) {
		error("Unsupported multiplexing protocol version %d "
		    "(expected %d)", ver, SSHMUX_VER);
		return -1;
	}
	debug2("%s: channel %d slave version %u", __func__, c->self, ver);

	 
	while (buffer_len(m) > 0) {
		char *name = buffer_get_string_ret(m, NULL);
		char *value = buffer_get_string_ret(m, NULL);

		if (name == NULL || value == NULL) {
			free(name);
			free(value);
			goto malf;
		}
		debug2("Unrecognised slave extension \"%s\"", name);
		free(name);
		free(value);
	}
	state->hello_rcvd = 1;
	return 0;
}
