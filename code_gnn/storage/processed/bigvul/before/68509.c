server_read_http(struct bufferevent *bev, void *arg)
{
	struct client		*clt = arg;
	struct server_config	*srv_conf = clt->clt_srv_conf;
	struct http_descriptor	*desc = clt->clt_descreq;
	struct evbuffer		*src = EVBUFFER_INPUT(bev);
	char			*line = NULL, *key, *value;
	const char		*errstr;
	size_t			 size, linelen;
	struct kv		*hdr = NULL;

	getmonotime(&clt->clt_tv_last);

	size = EVBUFFER_LENGTH(src);
	DPRINTF("%s: session %d: size %lu, to read %lld",
	    __func__, clt->clt_id, size, clt->clt_toread);
	if (!size) {
		clt->clt_toread = TOREAD_HTTP_HEADER;
		goto done;
	}

	while (!clt->clt_headersdone && (line =
	    evbuffer_readln(src, NULL, EVBUFFER_EOL_CRLF_STRICT)) != NULL) {
		linelen = strlen(line);

		 
		if (!linelen) {
			clt->clt_headersdone = 1;
			free(line);
			break;
		}
		key = line;

		 
		clt->clt_headerlen += linelen;
		if (clt->clt_headerlen > SERVER_MAXHEADERLENGTH) {
			server_abort_http(clt, 413, "request too large");
			goto abort;
		}

		 
		if (++clt->clt_line == 1)
			value = strchr(key, ' ');
		else if (*key == ' ' || *key == '\t')
			 
			value = NULL;
		else
			value = strchr(key, ':');
		if (value == NULL) {
			if (clt->clt_line == 1) {
				server_abort_http(clt, 400, "malformed");
				goto abort;
			}

			 
			if (kv_extend(&desc->http_headers,
			    desc->http_lastheader, line) == NULL)
				goto fail;

			free(line);
			continue;
		}
		if (*value == ':') {
			*value++ = '\0';
			value += strspn(value, " \t\r\n");
		} else {
			*value++ = '\0';
		}

		DPRINTF("%s: session %d: header '%s: %s'", __func__,
		    clt->clt_id, key, value);

		 
		if (clt->clt_line == 1) {
			if ((desc->http_method = server_httpmethod_byname(key))
			    == HTTP_METHOD_NONE) {
				server_abort_http(clt, 400, "malformed");
				goto abort;
			}

			 
			desc->http_path = strdup(value);
			if (desc->http_path == NULL)
				goto fail;

			desc->http_version = strchr(desc->http_path, ' ');
			if (desc->http_version == NULL) {
				server_abort_http(clt, 400, "malformed");
				goto abort;
			}

			*desc->http_version++ = '\0';
			desc->http_query = strchr(desc->http_path, '?');
			if (desc->http_query != NULL)
				*desc->http_query++ = '\0';

			 
			if ((desc->http_version =
			    strdup(desc->http_version)) == NULL)
				goto fail;

			if (desc->http_query != NULL &&
			    (desc->http_query =
			    strdup(desc->http_query)) == NULL)
				goto fail;

		} else if (desc->http_method != HTTP_METHOD_NONE &&
		    strcasecmp("Content-Length", key) == 0) {
			if (desc->http_method == HTTP_METHOD_TRACE ||
			    desc->http_method == HTTP_METHOD_CONNECT) {
				 
				server_abort_http(clt, 400, "malformed");
				goto abort;
			}

			 
			clt->clt_toread = strtonum(value, 0, LLONG_MAX,
			    &errstr);
			if (errstr) {
				server_abort_http(clt, 500, errstr);
				goto abort;
			}
			if ((size_t)clt->clt_toread >
			    srv_conf->maxrequestbody) {
				server_abort_http(clt, 413, NULL);
				goto abort;
			}
		}

		if (strcasecmp("Transfer-Encoding", key) == 0 &&
		    strcasecmp("chunked", value) == 0)
			desc->http_chunked = 1;

		if (clt->clt_line != 1) {
			if ((hdr = kv_add(&desc->http_headers, key,
			    value)) == NULL)
				goto fail;

			desc->http_lastheader = hdr;
		}

		free(line);
	}
	if (clt->clt_headersdone) {
		if (desc->http_method == HTTP_METHOD_NONE) {
			server_abort_http(clt, 406, "no method");
			return;
		}

		switch (desc->http_method) {
		case HTTP_METHOD_CONNECT:
			 
			clt->clt_toread = TOREAD_UNLIMITED;
			bev->readcb = server_read;
			break;
		case HTTP_METHOD_DELETE:
		case HTTP_METHOD_GET:
		case HTTP_METHOD_HEAD:
		 
		case HTTP_METHOD_COPY:
		case HTTP_METHOD_MOVE:
			clt->clt_toread = 0;
			break;
		case HTTP_METHOD_OPTIONS:
		case HTTP_METHOD_POST:
		case HTTP_METHOD_PUT:
		case HTTP_METHOD_RESPONSE:
		 
		case HTTP_METHOD_PROPFIND:
		case HTTP_METHOD_PROPPATCH:
		case HTTP_METHOD_MKCOL:
		case HTTP_METHOD_LOCK:
		case HTTP_METHOD_UNLOCK:
		case HTTP_METHOD_VERSION_CONTROL:
		case HTTP_METHOD_REPORT:
		case HTTP_METHOD_CHECKOUT:
		case HTTP_METHOD_CHECKIN:
		case HTTP_METHOD_UNCHECKOUT:
		case HTTP_METHOD_MKWORKSPACE:
		case HTTP_METHOD_UPDATE:
		case HTTP_METHOD_LABEL:
		case HTTP_METHOD_MERGE:
		case HTTP_METHOD_BASELINE_CONTROL:
		case HTTP_METHOD_MKACTIVITY:
		case HTTP_METHOD_ORDERPATCH:
		case HTTP_METHOD_ACL:
		case HTTP_METHOD_MKREDIRECTREF:
		case HTTP_METHOD_UPDATEREDIRECTREF:
		case HTTP_METHOD_SEARCH:
		case HTTP_METHOD_PATCH:
			 
			if (clt->clt_toread > 0)
				bev->readcb = server_read_httpcontent;

			 
			if (clt->clt_toread < 0) {
				clt->clt_toread = TOREAD_UNLIMITED;
				bev->readcb = server_read;
			}
			break;
		default:
			server_abort_http(clt, 405, "method not allowed");
			return;
		}
		if (desc->http_chunked) {
			 
			clt->clt_toread = TOREAD_HTTP_CHUNK_LENGTH;
			bev->readcb = server_read_httpchunks;
		}

 done:
		if (clt->clt_toread != 0)
			bufferevent_disable(bev, EV_READ);
		server_response(httpd_env, clt);
		return;
	}
	if (clt->clt_done) {
		server_close(clt, "done");
		return;
	}
	if (EVBUFFER_LENGTH(src) && bev->readcb != server_read_http)
		bev->readcb(bev, arg);
	bufferevent_enable(bev, EV_READ);
	return;
 fail:
	server_abort_http(clt, 500, strerror(errno));
 abort:
	free(line);
}
