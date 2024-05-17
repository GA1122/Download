server_read_httpchunks(struct bufferevent *bev, void *arg)
{
	struct client		*clt = arg;
	struct evbuffer		*src = EVBUFFER_INPUT(bev);
	char			*line;
	long long		 llval;
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
			if (server_bufferevent_write_chunk(clt, src, size)
			    == -1)
				goto fail;
			clt->clt_toread = 0;
		} else {
			if (server_bufferevent_write_buffer(clt, src) == -1)
				goto fail;
			clt->clt_toread -= size;
		}
		DPRINTF("%s: done, size %lu, to read %lld", __func__,
		    size, clt->clt_toread);
	}
	switch (clt->clt_toread) {
	case TOREAD_HTTP_CHUNK_LENGTH:
		line = evbuffer_readln(src, NULL, EVBUFFER_EOL_CRLF_STRICT);
		if (line == NULL) {
			 
			bufferevent_enable(bev, EV_READ);
			return;
		}
		if (strlen(line) == 0) {
			free(line);
			goto next;
		}

		 
		if (sscanf(line, "%llx", &llval) != 1 || llval < 0) {
			free(line);
			server_close(clt, "invalid chunk size");
			return;
		}

		if (server_bufferevent_print(clt, line) == -1 ||
		    server_bufferevent_print(clt, "\r\n") == -1) {
			free(line);
			goto fail;
		}
		free(line);

		if ((clt->clt_toread = llval) == 0) {
			DPRINTF("%s: last chunk", __func__);
			clt->clt_toread = TOREAD_HTTP_CHUNK_TRAILER;
		}
		break;
	case TOREAD_HTTP_CHUNK_TRAILER:
		 
		line = evbuffer_readln(src, NULL, EVBUFFER_EOL_CRLF_STRICT);
		if (line == NULL) {
			 
			bufferevent_enable(bev, EV_READ);
			return;
		}
		if (server_bufferevent_print(clt, line) == -1 ||
		    server_bufferevent_print(clt, "\r\n") == -1) {
			free(line);
			goto fail;
		}
		if (strlen(line) == 0) {
			 
			clt->clt_toread = TOREAD_HTTP_HEADER;
			bev->readcb = server_read_http;
		}
		free(line);
		break;
	case 0:
		 
		line = evbuffer_readln(src, NULL, EVBUFFER_EOL_CRLF_STRICT);
		free(line);
		if (server_bufferevent_print(clt, "\r\n") == -1)
			goto fail;
		clt->clt_toread = TOREAD_HTTP_CHUNK_LENGTH;
		break;
	}

 next:
	if (clt->clt_done)
		goto done;
	if (EVBUFFER_LENGTH(src))
		bev->readcb(bev, arg);
	bufferevent_enable(bev, EV_READ);
	return;

 done:
	server_close(clt, "last http chunk read (done)");
	return;
 fail:
 	server_close(clt, strerror(errno));
 }
