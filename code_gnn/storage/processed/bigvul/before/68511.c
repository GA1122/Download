server_read_httpcontent(struct bufferevent *bev, void *arg)
{
	struct client		*clt = arg;
	struct evbuffer		*src = EVBUFFER_INPUT(bev);
	size_t			 size;

	getmonotime(&clt->clt_tv_last);

	size = EVBUFFER_LENGTH(src);
	DPRINTF("%s: session %d: size %lu, to read %lld", __func__,
	    clt->clt_id, size, clt->clt_toread);
	if (!size)
		return;

	if (clt->clt_toread > 0) {
		 
		if ((off_t)size > clt->clt_toread) {
			size = clt->clt_toread;
			if (fcgi_add_stdin(clt, src) == -1)
				goto fail;
			clt->clt_toread = 0;
		} else {
			if (fcgi_add_stdin(clt, src) == -1)
				goto fail;
			clt->clt_toread -= size;
		}
		DPRINTF("%s: done, size %lu, to read %lld", __func__,
		    size, clt->clt_toread);
	}
	if (clt->clt_toread == 0) {
		fcgi_add_stdin(clt, NULL);
		clt->clt_toread = TOREAD_HTTP_HEADER;
		bufferevent_disable(bev, EV_READ);
		bev->readcb = server_read_http;
		return;
	}
	if (clt->clt_done)
		goto done;
	if (bev->readcb != server_read_httpcontent)
		bev->readcb(bev, arg);

	return;
 done:
	return;
 fail:
	server_close(clt, strerror(errno));
}