server_response_http(struct client *clt, unsigned int code,
    struct media_type *media, off_t size, time_t mtime)
{
	struct server_config	*srv_conf = clt->clt_srv_conf;
	struct http_descriptor	*desc = clt->clt_descreq;
	struct http_descriptor	*resp = clt->clt_descresp;
	const char		*error;
	struct kv		*ct, *cl;
	char			 tmbuf[32];

	if (desc == NULL || media == NULL ||
	    (error = server_httperror_byid(code)) == NULL)
		return (-1);

	if (server_log_http(clt, code, size) == -1)
		return (-1);

	 
	if (kv_setkey(&resp->http_pathquery, "%u", code) == -1 ||
	    kv_set(&resp->http_pathquery, "%s", error) == -1)
		return (-1);

	 
	if (kv_add(&resp->http_headers, "Server", HTTPD_SERVERNAME) == NULL)
		return (-1);

	 
	if (clt->clt_persist) {
		if (kv_add(&resp->http_headers,
		    "Connection", "keep-alive") == NULL)
			return (-1);
	} else if (kv_add(&resp->http_headers, "Connection", "close") == NULL)
		return (-1);

	 
	if ((ct = kv_add(&resp->http_headers, "Content-Type", NULL)) == NULL ||
	    kv_set(ct, "%s/%s", media->media_type, media->media_subtype) == -1)
		return (-1);

	 
	if ((cl =
	    kv_add(&resp->http_headers, "Content-Length", NULL)) == NULL ||
	    kv_set(cl, "%lld", (long long)size) == -1)
		return (-1);

	 
	if (server_http_time(mtime, tmbuf, sizeof(tmbuf)) <= 0 ||
	    kv_add(&resp->http_headers, "Last-Modified", tmbuf) == NULL)
		return (-1);

	 
	if (srv_conf->flags & SRVFLAG_SERVER_HSTS) {
		if ((cl =
		    kv_add(&resp->http_headers, "Strict-Transport-Security",
		    NULL)) == NULL ||
		    kv_set(cl, "max-age=%d%s%s", srv_conf->hsts_max_age,
		    srv_conf->hsts_flags & HSTSFLAG_SUBDOMAINS ?
		    "; includeSubDomains" : "",
		    srv_conf->hsts_flags & HSTSFLAG_PRELOAD ?
		    "; preload" : "") == -1)
			return (-1);
	}

	 
	if (server_http_time(time(NULL), tmbuf, sizeof(tmbuf)) <= 0 ||
	    kv_add(&resp->http_headers, "Date", tmbuf) == NULL)
		return (-1);

	 
	if (server_writeresponse_http(clt) == -1 ||
	    server_bufferevent_print(clt, "\r\n") == -1 ||
	    server_headers(clt, resp, server_writeheader_http, NULL) == -1 ||
	    server_bufferevent_print(clt, "\r\n") == -1)
		return (-1);

	if (size == 0 || resp->http_method == HTTP_METHOD_HEAD) {
		bufferevent_enable(clt->clt_bev, EV_READ|EV_WRITE);
		if (clt->clt_persist)
			clt->clt_toread = TOREAD_HTTP_HEADER;
		else
			clt->clt_toread = TOREAD_HTTP_NONE;
		clt->clt_done = 0;
		return (0);
	}

	return (1);
}