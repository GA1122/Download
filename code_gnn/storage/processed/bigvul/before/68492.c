server_expand_http(struct client *clt, const char *val, char *buf,
    size_t len)
{
	struct http_descriptor	*desc = clt->clt_descreq;
	struct server_config	*srv_conf = clt->clt_srv_conf;
	char			 ibuf[128], *str, *path, *query;
	const char		*errstr = NULL, *p;
	size_t			 size;
	int			 n, ret;

	if (strlcpy(buf, val, len) >= len)
		return (NULL);

	 
	for (p = val; clt->clt_srv_match.sm_nmatch &&
	    (p = strstr(p, "%")) != NULL; p++) {
		if (!isdigit((unsigned char)*(p + 1)))
			continue;

		 
		size = strspn(p + 1, "0123456789") + 2;
		if (size  >= sizeof(ibuf))
			return (NULL);
		(void)strlcpy(ibuf, p, size);
		n = strtonum(ibuf + 1, 0,
		    clt->clt_srv_match.sm_nmatch - 1, &errstr);
		if (errstr != NULL)
			return (NULL);

		 
		if ((str = url_encode(clt->clt_srv_match.sm_match[n])) == NULL)
			return (NULL);
		ret = expand_string(buf, len, ibuf, str);
		free(str);
		if (ret != 0)
			return (NULL);
	}
	if (strstr(val, "$DOCUMENT_URI") != NULL) {
		if ((path = url_encode(desc->http_path)) == NULL)
			return (NULL);
		ret = expand_string(buf, len, "$DOCUMENT_URI", path);
		free(path);
		if (ret != 0)
			return (NULL);
	}
	if (strstr(val, "$QUERY_STRING") != NULL) {
		if (desc->http_query == NULL) {
			ret = expand_string(buf, len, "$QUERY_STRING", "");
		} else {
			if ((query = url_encode(desc->http_query)) == NULL)
				return (NULL);
			ret = expand_string(buf, len, "$QUERY_STRING", query);
			free(query);
		}
		if (ret != 0)
			return (NULL);
	}
	if (strstr(val, "$REMOTE_") != NULL) {
		if (strstr(val, "$REMOTE_ADDR") != NULL) {
			if (print_host(&clt->clt_ss,
			    ibuf, sizeof(ibuf)) == NULL)
				return (NULL);
			if (expand_string(buf, len,
			    "$REMOTE_ADDR", ibuf) != 0)
				return (NULL);
		}
		if (strstr(val, "$REMOTE_PORT") != NULL) {
			snprintf(ibuf, sizeof(ibuf),
			    "%u", ntohs(clt->clt_port));
			if (expand_string(buf, len,
			    "$REMOTE_PORT", ibuf) != 0)
				return (NULL);
		}
		if (strstr(val, "$REMOTE_USER") != NULL) {
			if ((srv_conf->flags & SRVFLAG_AUTH) &&
			    clt->clt_remote_user != NULL) {
				if ((str = url_encode(clt->clt_remote_user))
				    == NULL)
					return (NULL);
			} else
				str = strdup("");
			ret = expand_string(buf, len, "$REMOTE_USER", str);
			free(str);
			if (ret != 0)
				return (NULL);
		}
	}
	if (strstr(val, "$REQUEST_URI") != NULL) {
		if ((path = url_encode(desc->http_path)) == NULL)
			return (NULL);
		if (desc->http_query == NULL) {
			str = path;
		} else {
			if ((query = url_encode(desc->http_query)) == NULL) {
				free(path);
				return (NULL);
			}
			ret = asprintf(&str, "%s?%s", path, query);
			free(path);
			free(query);
			if (ret == -1)
				return (NULL);
		}

		ret = expand_string(buf, len, "$REQUEST_URI", str);
		free(str);
		if (ret != 0)
			return (NULL);
	}
	if (strstr(val, "$SERVER_") != NULL) {
		if (strstr(val, "$SERVER_ADDR") != NULL) {
			if (print_host(&srv_conf->ss,
			    ibuf, sizeof(ibuf)) == NULL)
				return (NULL);
			if (expand_string(buf, len,
			    "$SERVER_ADDR", ibuf) != 0)
				return (NULL);
		}
		if (strstr(val, "$SERVER_PORT") != NULL) {
			snprintf(ibuf, sizeof(ibuf), "%u",
			    ntohs(srv_conf->port));
			if (expand_string(buf, len,
			    "$SERVER_PORT", ibuf) != 0)
				return (NULL);
		}
		if (strstr(val, "$SERVER_NAME") != NULL) {
			if ((str = url_encode(srv_conf->name))
			     == NULL)
				return (NULL);
			ret = expand_string(buf, len, "$SERVER_NAME", str);
			free(str);
			if (ret != 0)
				return (NULL);
		}
	}

	return (buf);
}