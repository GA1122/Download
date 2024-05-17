mux_confirm_remote_forward(int type, u_int32_t seq, void *ctxt)
{
	struct mux_channel_confirm_ctx *fctx = ctxt;
	char *failmsg = NULL;
	struct Forward *rfwd;
	Channel *c;
	Buffer out;

	if ((c = channel_by_id(fctx->cid)) == NULL) {
		 
		error("%s: unknown channel", __func__);
		return;
	}
	buffer_init(&out);
	if (fctx->fid >= options.num_remote_forwards ||
	    (options.remote_forwards[fctx->fid].connect_path == NULL &&
	    options.remote_forwards[fctx->fid].connect_host == NULL)) {
		xasprintf(&failmsg, "unknown forwarding id %d", fctx->fid);
		goto fail;
	}
	rfwd = &options.remote_forwards[fctx->fid];
	debug("%s: %s for: listen %d, connect %s:%d", __func__,
	    type == SSH2_MSG_REQUEST_SUCCESS ? "success" : "failure",
	    rfwd->listen_port, rfwd->connect_path ? rfwd->connect_path :
	    rfwd->connect_host, rfwd->connect_port);
	if (type == SSH2_MSG_REQUEST_SUCCESS) {
		if (rfwd->listen_port == 0) {
			rfwd->allocated_port = packet_get_int();
			debug("Allocated port %u for mux remote forward"
			    " to %s:%d", rfwd->allocated_port,
			    rfwd->connect_host, rfwd->connect_port);
			buffer_put_int(&out, MUX_S_REMOTE_PORT);
			buffer_put_int(&out, fctx->rid);
			buffer_put_int(&out, rfwd->allocated_port);
			channel_update_permitted_opens(rfwd->handle,
			   rfwd->allocated_port);
		} else {
			buffer_put_int(&out, MUX_S_OK);
			buffer_put_int(&out, fctx->rid);
		}
		goto out;
	} else {
		if (rfwd->listen_port == 0)
			channel_update_permitted_opens(rfwd->handle, -1);
		if (rfwd->listen_path != NULL)
			xasprintf(&failmsg, "remote port forwarding failed for "
			    "listen path %s", rfwd->listen_path);
		else
			xasprintf(&failmsg, "remote port forwarding failed for "
			    "listen port %d", rfwd->listen_port);

                debug2("%s: clearing registered forwarding for listen %d, "
		    "connect %s:%d", __func__, rfwd->listen_port,
		    rfwd->connect_path ? rfwd->connect_path :
		    rfwd->connect_host, rfwd->connect_port);

		free(rfwd->listen_host);
		free(rfwd->listen_path);
		free(rfwd->connect_host);
		free(rfwd->connect_path);
		memset(rfwd, 0, sizeof(*rfwd));
	}
 fail:
	error("%s: %s", __func__, failmsg);
	buffer_put_int(&out, MUX_S_FAILURE);
	buffer_put_int(&out, fctx->rid);
	buffer_put_cstring(&out, failmsg);
	free(failmsg);
 out:
	buffer_put_string(&c->output, buffer_ptr(&out), buffer_len(&out));
	buffer_free(&out);
	if (c->mux_pause <= 0)
		fatal("%s: mux_pause %d", __func__, c->mux_pause);
	c->mux_pause = 0;  
}
