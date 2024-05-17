server_response(struct httpd *httpd, struct client *clt)
{
	char			 path[PATH_MAX];
	char			 hostname[HOST_NAME_MAX+1];
	struct http_descriptor	*desc = clt->clt_descreq;
	struct http_descriptor	*resp = clt->clt_descresp;
	struct server		*srv = clt->clt_srv;
	struct server_config	*srv_conf = &srv->srv_conf;
	struct kv		*kv, key, *host;
	struct str_find		 sm;
	int			 portval = -1, ret;
	char			*hostval;
	const char		*errstr = NULL;

	 
	if (desc->http_path == NULL ||
	    url_decode(desc->http_path) == NULL ||
	    canonicalize_path(desc->http_path, path, sizeof(path)) == NULL)
		goto fail;
	free(desc->http_path);
	if ((desc->http_path = strdup(path)) == NULL)
		goto fail;

	key.kv_key = "Host";
	if ((host = kv_find(&desc->http_headers, &key)) != NULL &&
	    host->kv_value == NULL)
		host = NULL;

	if (strcmp(desc->http_version, "HTTP/1.1") == 0) {
		 
		if (host == NULL)
			goto fail;

		 
		key.kv_key = "Connection";
		if ((kv = kv_find(&desc->http_headers, &key)) != NULL &&
		    strcasecmp("close", kv->kv_value) == 0)
			clt->clt_persist = 0;
		else
			clt->clt_persist++;
	} else {
		 
		key.kv_key = "Connection";
		if ((kv = kv_find(&desc->http_headers, &key)) != NULL &&
		    strcasecmp("keep-alive", kv->kv_value) == 0)
			clt->clt_persist++;
		else
			clt->clt_persist = 0;
	}

	if (clt->clt_persist >= srv_conf->maxrequests)
		clt->clt_persist = 0;

	 
	if (host != NULL) {
		if ((hostval = server_http_parsehost(host->kv_value,
		    hostname, sizeof(hostname), &portval)) == NULL)
			goto fail;

		TAILQ_FOREACH(srv_conf, &srv->srv_hosts, entry) {
#ifdef DEBUG
			if ((srv_conf->flags & SRVFLAG_LOCATION) == 0) {
				DPRINTF("%s: virtual host \"%s:%u\""
				    " host \"%s\" (\"%s\")",
				    __func__, srv_conf->name,
				    ntohs(srv_conf->port), host->kv_value,
				    hostname);
			}
#endif
			if (srv_conf->flags & SRVFLAG_LOCATION)
				continue;
			else if (srv_conf->flags & SRVFLAG_SERVER_MATCH) {
				str_find(hostname, srv_conf->name,
				    &sm, 1, &errstr);
				ret = errstr == NULL ? 0 : -1;
			} else {
				ret = fnmatch(srv_conf->name,
				    hostname, FNM_CASEFOLD);
			}
			if (ret == 0 &&
			    (portval == -1 ||
			    (portval != -1 && portval == srv_conf->port))) {
				 
				clt->clt_srv_conf = srv_conf;
				srv_conf = NULL;
				break;
			}
		}
	}

	if (srv_conf != NULL) {
		 
		if (server_http_host(&clt->clt_srv_ss, hostname,
		    sizeof(hostname)) == NULL)
			goto fail;
	} else {
		 
		if (strlcpy(hostname, host->kv_value, sizeof(hostname)) >=
		    sizeof(hostname))
			goto fail;
		srv_conf = clt->clt_srv_conf;
	}

	if ((desc->http_host = strdup(hostname)) == NULL)
		goto fail;

	 
	resp->http_method = desc->http_method;
	if ((resp->http_version = strdup(desc->http_version)) == NULL)
		goto fail;

	 
	srv_conf = server_getlocation(clt, desc->http_path);

	if (srv_conf->flags & SRVFLAG_BLOCK) {
		server_abort_http(clt, srv_conf->return_code,
		    srv_conf->return_uri);
		return (-1);
	} else if (srv_conf->flags & SRVFLAG_AUTH &&
	    server_http_authenticate(srv_conf, clt) == -1) {
		server_abort_http(clt, 401, srv_conf->auth_realm);
		return (-1);
	} else
		return (server_file(httpd, clt));
 fail:
	server_abort_http(clt, 400, "bad request");
	return (-1);
}