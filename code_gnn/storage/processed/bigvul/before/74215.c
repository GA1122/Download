unpeer_name_resolved(
	int			rescode,
	int			gai_errno,
	void *			context,
	const char *		name,
	const char *		service,
	const struct addrinfo *	hints,
	const struct addrinfo *	res
	)
{
	sockaddr_u	peeraddr;
	struct peer *	peer;
	u_short		af;
	const char *	fam_spec;

	DPRINTF(1, ("unpeer_name_resolved(%s) rescode %d\n", name, rescode));

	if (rescode) {
		msyslog(LOG_ERR, "giving up resolving unpeer %s: %s (%d)", 
			name, gai_strerror(rescode), rescode);
		return;
	}
	 
	for (; res != NULL; res = res->ai_next) {
		NTP_INSIST(res->ai_addrlen <= sizeof(peeraddr));
		memcpy(&peeraddr, res->ai_addr, res->ai_addrlen);
		DPRINTF(1, ("unpeer: searching for peer %s\n",
			    stoa(&peeraddr)));
		peer = findexistingpeer(&peeraddr, NULL, NULL, -1);
		if (peer != NULL) {
			af = AF(&peeraddr);
			fam_spec = (AF_INET6 == af)
				       ? "(AAAA) "
				       : (AF_INET == af)
					     ? "(A) "
					     : "";
			msyslog(LOG_NOTICE, "unpeered %s %s-> %s", name,
				fam_spec, stoa(&peeraddr));
			peer_clear(peer, "GONE");
			unpeer(peer);
		}
	}
}