resolve_addr(const char *name, int port, char *caddr, size_t clen)
{
	char addr[NI_MAXHOST], strport[NI_MAXSERV];
	struct addrinfo hints, *res;
	int gaierr;

	if (port <= 0)
		port = default_ssh_port();
	snprintf(strport, sizeof strport, "%u", port);
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = options.address_family == -1 ?
	    AF_UNSPEC : options.address_family;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICHOST|AI_NUMERICSERV;
	if ((gaierr = getaddrinfo(name, strport, &hints, &res)) != 0) {
		debug2("%s: could not resolve name %.100s as address: %s",
		    __func__, name, ssh_gai_strerror(gaierr));
		return NULL;
	}
	if (res == NULL) {
		debug("%s: getaddrinfo %.100s returned no addresses",
		 __func__, name);
		return NULL;
	}
	if (res->ai_next != NULL) {
		debug("%s: getaddrinfo %.100s returned multiple addresses",
		    __func__, name);
		goto fail;
	}
	if ((gaierr = getnameinfo(res->ai_addr, res->ai_addrlen,
	    addr, sizeof(addr), NULL, 0, NI_NUMERICHOST)) != 0) {
		debug("%s: Could not format address for name %.100s: %s",
		    __func__, name, ssh_gai_strerror(gaierr));
		goto fail;
	}
	if (strlcpy(caddr, addr, clen) >= clen) {
		error("%s: host \"%s\" addr \"%s\" too long (max %lu)",
		    __func__, name,  addr, (u_long)clen);
		if (clen > 0)
			*caddr = '\0';
 fail:
		freeaddrinfo(res);
		return NULL;
	}
	return res;
}
