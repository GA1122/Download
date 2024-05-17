alloc_get_host(struct mg_connection *conn)
{
	char buf[1025];
	size_t buflen = sizeof(buf);
	const char *host_header = get_header(conn->request_info.http_headers,
	                                     conn->request_info.num_headers,
	                                     "Host");
	char *host;

	if (host_header != NULL) {
		char *pos;

		 
		mg_strlcpy(buf, host_header, buflen);
		buf[buflen - 1] = '\0';
		host = buf;
		while (isspace(*host)) {
			host++;
		}

		 
		if (*host == '[') {
			pos = strchr(host, ']');
			if (!pos) {
				 
				DEBUG_TRACE("%s", "Host name format error '[' without ']'");
				return NULL;
			}
			 
			pos[1] = 0;
		} else {
			 
			pos = strchr(host, ':');
			if (pos != NULL) {
				*pos = '\0';
			}
		}

		if (conn->ssl) {
			 
			const char *sslhost = conn->dom_ctx->config[AUTHENTICATION_DOMAIN];
			if (sslhost && (conn->dom_ctx != &(conn->phys_ctx->dd))) {
				 
				if (mg_strcasecmp(host, sslhost)) {
					 
					DEBUG_TRACE("Host mismatch: SNI: %s, HTTPS: %s",
					            sslhost,
					            host);
					return NULL;
				}
			}
			DEBUG_TRACE("HTTPS Host: %s", host);

		} else {
			struct mg_domain_context *dom = &(conn->phys_ctx->dd);
			while (dom) {
				if (!mg_strcasecmp(host, dom->config[AUTHENTICATION_DOMAIN])) {

					 
					DEBUG_TRACE("HTTP domain %s found",
					            dom->config[AUTHENTICATION_DOMAIN]);

					 
					conn->dom_ctx = dom;
					break;
				}
				dom = dom->next;
			}

			DEBUG_TRACE("HTTP Host: %s", host);
		}

	} else {
		sockaddr_to_string(buf, buflen, &conn->client.lsa);
		host = buf;

		DEBUG_TRACE("IP: %s", host);
	}

	return mg_strdup_ctx(host, conn->phys_ctx);
}
