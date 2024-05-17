get_rel_url_at_current_server(const char *uri, const struct mg_connection *conn)
{
	const char *server_domain;
	size_t server_domain_len;
	size_t request_domain_len = 0;
	unsigned long port = 0;
	int i, auth_domain_check_enabled;
	const char *hostbegin = NULL;
	const char *hostend = NULL;
	const char *portbegin;
	char *portend;

	auth_domain_check_enabled =
	    !mg_strcasecmp(conn->dom_ctx->config[ENABLE_AUTH_DOMAIN_CHECK], "yes");

	 
	for (i = 0; abs_uri_protocols[i].proto != NULL; i++) {
		if (mg_strncasecmp(uri,
		                   abs_uri_protocols[i].proto,
		                   abs_uri_protocols[i].proto_len) == 0) {

			hostbegin = uri + abs_uri_protocols[i].proto_len;
			hostend = strchr(hostbegin, '/');
			if (!hostend) {
				return 0;
			}
			portbegin = strchr(hostbegin, ':');
			if ((!portbegin) || (portbegin > hostend)) {
				port = abs_uri_protocols[i].default_port;
				request_domain_len = (size_t)(hostend - hostbegin);
			} else {
				port = strtoul(portbegin + 1, &portend, 10);
				if ((portend != hostend) || (port <= 0)
				    || !is_valid_port(port)) {
					return 0;
				}
				request_domain_len = (size_t)(portbegin - hostbegin);
			}
			 
			break;
		}
	}

	if (!port) {
		 
		return 0;
	}

 
 
#if defined(USE_IPV6)
	if (conn->client.lsa.sa.sa_family == AF_INET6) {
		if (ntohs(conn->client.lsa.sin6.sin6_port) != port) {
			 
			return 0;
		}
	} else
#endif
	{
		if (ntohs(conn->client.lsa.sin.sin_port) != port) {
			 
			return 0;
		}
	}

	 
	if (auth_domain_check_enabled) {
		server_domain = conn->dom_ctx->config[AUTHENTICATION_DOMAIN];
		server_domain_len = strlen(server_domain);
		if ((server_domain_len == 0) || (hostbegin == NULL)) {
			return 0;
		}
		if ((request_domain_len == server_domain_len)
		    && (!memcmp(server_domain, hostbegin, server_domain_len))) {
			 
		} else {
			if (request_domain_len < (server_domain_len + 2)) {
				 
				return 0;
			}
			if (hostbegin[request_domain_len - server_domain_len - 1] != '.') {
				 
				return 0;
			}
			if (0 != memcmp(server_domain,
			                hostbegin + request_domain_len - server_domain_len,
			                server_domain_len)) {
				 
				return 0;
			}
		}
	}

	return hostend;
}
