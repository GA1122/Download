server_file_error(struct bufferevent *bev, short error, void *arg)
{
	struct client		*clt = arg;
	struct evbuffer		*dst;

	if (error & EVBUFFER_TIMEOUT) {
		server_close(clt, "buffer event timeout");
		return;
	}
	if (error & EVBUFFER_ERROR) {
		if (errno == EFBIG) {
			bufferevent_enable(bev, EV_READ);
			return;
		}
		server_close(clt, "buffer event error");
		return;
	}
	if (error & (EVBUFFER_READ|EVBUFFER_WRITE|EVBUFFER_EOF)) {
		bufferevent_disable(bev, EV_READ|EV_WRITE);

		clt->clt_done = 1;

		if (clt->clt_persist) {
			 
			if (clt->clt_fd != -1)
				close(clt->clt_fd);
			clt->clt_fd = -1;
			clt->clt_toread = TOREAD_HTTP_HEADER;
			server_reset_http(clt);
			bufferevent_enable(clt->clt_bev, EV_READ|EV_WRITE);
			return;
		}

		dst = EVBUFFER_OUTPUT(clt->clt_bev);
		if (EVBUFFER_LENGTH(dst)) {
			 
			bufferevent_enable(clt->clt_bev, EV_WRITE);
			return;
		}

		server_close(clt, "done");
		return;
	}
	server_close(clt, "unknown event error");
	return;
}