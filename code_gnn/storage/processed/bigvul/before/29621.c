static int sctp_inet6_af_supported(sa_family_t family, struct sctp_sock *sp)
{
	switch (family) {
	case AF_INET6:
		return 1;
	 
	case AF_INET:
		if (!__ipv6_only_sock(sctp_opt2sk(sp)))
			return 1;
	default:
		return 0;
	}
}