peer_name_resolved(
	int			rescode,
	int			gai_errno,
	void *			context,
	const char *		name,
	const char *		service,
	const struct addrinfo *	hints,
	const struct addrinfo *	res
	)
{
	sockaddr_u		peeraddr;
	peer_resolved_ctx *	ctx;
	u_short			af;
	const char *		fam_spec;

	ctx = context;

	DPRINTF(1, ("peer_name_resolved(%s) rescode %d\n", name, rescode));

	if (rescode) {
#ifndef IGNORE_DNS_ERRORS
		free(ctx);
		msyslog(LOG_ERR,
			"giving up resolving host %s: %s (%d)",
			name, gai_strerror(rescode), rescode);
#else	 
		getaddrinfo_sometime(name, service, hints,
				     INITIAL_DNS_RETRY,
				     &peer_name_resolved, context);
#endif
		return;
	}

	 
	for (; res != NULL; res = res->ai_next) {
		memcpy(&peeraddr, res->ai_addr, res->ai_addrlen);
		if (is_sane_resolved_address(&peeraddr,
					     ctx->host_mode)) {
			NLOG(NLOG_SYSINFO) {
				af = ctx->family;
				fam_spec = (AF_INET6 == af)
					       ? "(AAAA) "
					       : (AF_INET == af)
						     ? "(A) "
						     : "";
				msyslog(LOG_INFO, "DNS %s %s-> %s",
					name, fam_spec,
					stoa(&peeraddr));
			}
			peer_config(
				&peeraddr,
				NULL,
				NULL,
				ctx->hmode,
				ctx->version,
				ctx->minpoll,
				ctx->maxpoll,
				ctx->flags,
				ctx->ttl,
				ctx->keyid,
				ctx->group);
			break;
		}
	}
	free(ctx);
}