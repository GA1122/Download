static int netsnmp_session_init(php_snmp_session **session_p, int version, char *hostname, char *community, int timeout, int retries)
{
	php_snmp_session *session;
	char *pptr, *host_ptr;
	int force_ipv6 = FALSE;
	int n;
	struct sockaddr **psal;
	struct sockaddr **res;

	*session_p = (php_snmp_session *)emalloc(sizeof(php_snmp_session));
	session = *session_p;
	if (session == NULL) {
		php_error_docref(NULL, E_WARNING, "emalloc() failed allocating session");
		return (-1);
	}
	memset(session, 0, sizeof(php_snmp_session));

	snmp_sess_init(session);

	session->version = version;
	session->remote_port = SNMP_PORT;

	session->peername = emalloc(MAX_NAME_LEN);
	if (session->peername == NULL) {
		php_error_docref(NULL, E_WARNING, "emalloc() failed while copying hostname");
		return (-1);
	}
	 
	strlcpy(session->peername, hostname, MAX_NAME_LEN);
	host_ptr = session->peername;

	 
	if (*host_ptr == '[') {  
		force_ipv6 = TRUE;
		host_ptr++;
		if ((pptr = strchr(host_ptr, ']'))) {
			if (pptr[1] == ':') {
				session->remote_port = atoi(pptr + 2);
			}
			*pptr = '\0';
		} else {
			php_error_docref(NULL, E_WARNING, "malformed IPv6 address, closing square bracket missing");
			return (-1);
		}
	} else {  
		if ((pptr = strchr(host_ptr, ':'))) {
			session->remote_port = atoi(pptr + 1);
			*pptr = '\0';
		}
	}

	 
	if ((n = php_network_getaddresses(host_ptr, SOCK_DGRAM, &psal, NULL)) == 0) {  
		 
		return (-1);
	}

	 
	*(session->peername) = '\0';
	res = psal;
	while (n-- > 0) {
		pptr = session->peername;
#if HAVE_GETADDRINFO && HAVE_IPV6 && HAVE_INET_NTOP
		if (force_ipv6 && (*res)->sa_family != AF_INET6) {
			res++;
			continue;
		}
		if ((*res)->sa_family == AF_INET6) {
			strcpy(session->peername, "udp6:[");
			pptr = session->peername + strlen(session->peername);
			inet_ntop((*res)->sa_family, &(((struct sockaddr_in6*)(*res))->sin6_addr), pptr, MAX_NAME_LEN);
			strcat(pptr, "]");
		} else if ((*res)->sa_family == AF_INET) {
			inet_ntop((*res)->sa_family, &(((struct sockaddr_in*)(*res))->sin_addr), pptr, MAX_NAME_LEN);
		} else {
			res++;
			continue;
		}
#else
		if ((*res)->sa_family != AF_INET) {
			res++;
			continue;
		}
		strcat(pptr, inet_ntoa(((struct sockaddr_in*)(*res))->sin_addr));
#endif
		break;
	}

	if (strlen(session->peername) == 0) {
		php_error_docref(NULL, E_WARNING, "Unknown failure while resolving '%s'", hostname);
		return (-1);
	}
	 

	 
	if (session->remote_port != SNMP_PORT) {
		pptr = session->peername + strlen(session->peername);
		sprintf(pptr, ":%d", session->remote_port);
	}

	php_network_freeaddresses(psal);

	if (version == SNMP_VERSION_3) {
		 
		session->securityName = estrdup(community);
		session->securityNameLen = strlen(session->securityName);
	} else {
		session->authenticator = NULL;
		session->community = (u_char *)estrdup(community);
		session->community_len = strlen(community);
	}

	session->retries = retries;
	session->timeout = timeout;
	return (0);
}
