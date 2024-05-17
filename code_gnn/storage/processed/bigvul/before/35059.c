static struct sctp_af *sctp_sockaddr_af(struct sctp_sock *opt,
					union sctp_addr *addr, int len)
{
	struct sctp_af *af;

	 
	if (len < sizeof (struct sockaddr))
		return NULL;

	 
	if (!opt->pf->af_supported(addr->sa.sa_family, opt))
		return NULL;

	 
	af = sctp_get_af_specific(addr->sa.sa_family);

	if (len < af->sockaddr_len)
		return NULL;

	return af;
}
