mux_stdio_confirm(int id, int success, void *arg)
{
	struct mux_stdio_confirm_ctx *cctx = arg;
	Channel *c, *cc;
	Buffer reply;

	if (cctx == NULL)
		fatal("%s: cctx == NULL", __func__);
	if ((c = channel_by_id(id)) == NULL)
		fatal("%s: no channel for id %d", __func__, id);
	if ((cc = channel_by_id(c->ctl_chan)) == NULL)
		fatal("%s: channel %d lacks control channel %d", __func__,
		    id, c->ctl_chan);

	if (!success) {
		debug3("%s: sending failure reply", __func__);
		 
		buffer_init(&reply);
		buffer_put_int(&reply, MUX_S_FAILURE);
		buffer_put_int(&reply, cctx->rid);
		buffer_put_cstring(&reply, "Session open refused by peer");
		goto done;
	}

	debug3("%s: sending success reply", __func__);
	 
	buffer_init(&reply);
	buffer_put_int(&reply, MUX_S_SESSION_OPENED);
	buffer_put_int(&reply, cctx->rid);
	buffer_put_int(&reply, c->self);

 done:
	 
	buffer_put_string(&cc->output, buffer_ptr(&reply), buffer_len(&reply));
	buffer_free(&reply);

	if (cc->mux_pause <= 0)
		fatal("%s: mux_pause %d", __func__, cc->mux_pause);
	cc->mux_pause = 0;  
	c->open_confirm_ctx = NULL;
	free(cctx);
}
