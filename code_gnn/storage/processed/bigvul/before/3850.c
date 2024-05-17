svc_dg_enable_pktinfo(int fd, const struct __rpc_sockinfo *si)
{
	int val = 1;

	switch (si->si_af) {
	case AF_INET:
		(void) setsockopt(fd, SOL_IP, IP_PKTINFO, &val, sizeof(val));
		break;
#ifdef INET6
	case AF_INET6:
		(void) setsockopt(fd, SOL_IPV6, IPV6_RECVPKTINFO, &val, sizeof(val));
		break;
#endif
	}
}
