mux_master_read_cb(Channel *c)
{
	struct mux_master_state *state = (struct mux_master_state *)c->mux_ctx;
	Buffer in, out;
	const u_char *ptr;
	u_int type, rid, have, i;
	int ret = -1;

	 
	if (c->mux_ctx == NULL) {
		state = xcalloc(1, sizeof(*state));
		c->mux_ctx = state;
		channel_register_cleanup(c->self,
		    mux_master_control_cleanup_cb, 0);

		 
		buffer_init(&out);
		buffer_put_int(&out, MUX_MSG_HELLO);
		buffer_put_int(&out, SSHMUX_VER);
		 
		buffer_put_string(&c->output, buffer_ptr(&out),
		    buffer_len(&out));
		buffer_free(&out);
		debug3("%s: channel %d: hello sent", __func__, c->self);
		return 0;
	}

	buffer_init(&in);
	buffer_init(&out);

	 
	if ((ptr = buffer_get_string_ptr_ret(&c->input, &have)) == NULL) {
 malf:
		error("%s: malformed message", __func__);
		goto out;
	}
	buffer_append(&in, ptr, have);

	if (buffer_get_int_ret(&type, &in) != 0)
		goto malf;
	debug3("%s: channel %d packet type 0x%08x len %u",
	    __func__, c->self, type, buffer_len(&in));

	if (type == MUX_MSG_HELLO)
		rid = 0;
	else {
		if (!state->hello_rcvd) {
			error("%s: expected MUX_MSG_HELLO(0x%08x), "
			    "received 0x%08x", __func__, MUX_MSG_HELLO, type);
			goto out;
		}
		if (buffer_get_int_ret(&rid, &in) != 0)
			goto malf;
	}

	for (i = 0; mux_master_handlers[i].handler != NULL; i++) {
		if (type == mux_master_handlers[i].type) {
			ret = mux_master_handlers[i].handler(rid, c, &in, &out);
			break;
		}
	}
	if (mux_master_handlers[i].handler == NULL) {
		error("%s: unsupported mux message 0x%08x", __func__, type);
		buffer_put_int(&out, MUX_S_FAILURE);
		buffer_put_int(&out, rid);
		buffer_put_cstring(&out, "unsupported request");
		ret = 0;
	}
	 
	if (buffer_len(&out) != 0) {
		buffer_put_string(&c->output, buffer_ptr(&out),
		    buffer_len(&out));
	}
 out:
	buffer_free(&in);
	buffer_free(&out);
	return ret;
}
