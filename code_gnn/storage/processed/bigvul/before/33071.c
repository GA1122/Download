static struct sctp_af *sctp_sockaddr_af(struct sctp_sock *opt,
					union sctp_addr *addr, int len)
{
	struct sctp_af *af;

	 
	if (len < sizeof (struct sockaddr))
		return NULL;

	 
	if (addr->sa.sa_family == AF_INET6 &&
	    ipv6_addr_v4mapped(&addr->v6.sin6_addr)) {
		if (!opt->pf->af_supported(AF_INET, opt))
			return NULL;
	} else {
		 
		if (!opt->pf->af_supported(addr->sa.sa_family, opt))
			return NULL;
	}

	 
	af = sctp_get_af_specific(addr->sa.sa_family);

	if (len < af->sockaddr_len)
		return NULL;

	return af;
}
