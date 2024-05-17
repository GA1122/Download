static int sctp_inet6_cmp_addr(const union sctp_addr *addr1,
			       const union sctp_addr *addr2,
			       struct sctp_sock *opt)
{
	struct sctp_af *af1, *af2;
	struct sock *sk = sctp_opt2sk(opt);

	af1 = sctp_get_af_specific(addr1->sa.sa_family);
	af2 = sctp_get_af_specific(addr2->sa.sa_family);

	if (!af1 || !af2)
		return 0;

	 
	if (__ipv6_only_sock(sk) && af1 != af2)
		return 0;

	 
	if (sctp_is_any(sk, addr1) || sctp_is_any(sk, addr2))
		return 1;

	if (addr1->sa.sa_family != addr2->sa.sa_family)
		return 0;

	return af1->cmp_addr(addr1, addr2);
}