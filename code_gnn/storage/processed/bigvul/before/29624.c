static void sctp_inet6_event_msgname(struct sctp_ulpevent *event,
				     char *msgname, int *addrlen)
{
	struct sockaddr_in6 *sin6, *sin6from;

	if (msgname) {
		union sctp_addr *addr;
		struct sctp_association *asoc;

		asoc = event->asoc;
		sctp_inet6_msgname(msgname, addrlen);
		sin6 = (struct sockaddr_in6 *)msgname;
		sin6->sin6_port = htons(asoc->peer.port);
		addr = &asoc->peer.primary_addr;

		 

		 
		if (sctp_sk(asoc->base.sk)->v4mapped &&
		    AF_INET == addr->sa.sa_family) {
			sctp_v4_map_v6((union sctp_addr *)sin6);
			sin6->sin6_addr.s6_addr32[3] =
				addr->v4.sin_addr.s_addr;
			return;
		}

		sin6from = &asoc->peer.primary_addr.v6;
		sin6->sin6_addr = sin6from->sin6_addr;
		if (ipv6_addr_type(&sin6->sin6_addr) & IPV6_ADDR_LINKLOCAL)
			sin6->sin6_scope_id = sin6from->sin6_scope_id;
	}
}