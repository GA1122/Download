__xprt_set_raddr(SVCXPRT *xprt, const struct sockaddr_storage *ss)
{
	switch (ss->ss_family) {
	case AF_INET6:
		memcpy(&xprt->xp_raddr, ss, sizeof(struct sockaddr_in6));
		xprt->xp_addrlen = sizeof (struct sockaddr_in6);
		break;
	case AF_INET:
		memcpy(&xprt->xp_raddr, ss, sizeof(struct sockaddr_in));
		xprt->xp_addrlen = sizeof (struct sockaddr_in);
		break;
	default:
		xprt->xp_raddr.sin6_family = AF_UNSPEC;
		xprt->xp_addrlen = sizeof (struct sockaddr);
		break;
	}
}
