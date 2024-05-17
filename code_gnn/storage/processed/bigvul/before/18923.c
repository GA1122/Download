int compat_ip_setsockopt(struct sock *sk, int level, int optname,
			 char __user *optval, unsigned int optlen)
{
	int err;

	if (level != SOL_IP)
		return -ENOPROTOOPT;

	if (optname >= MCAST_JOIN_GROUP && optname <= MCAST_MSFILTER)
		return compat_mc_setsockopt(sk, level, optname, optval, optlen,
			ip_setsockopt);

	err = do_ip_setsockopt(sk, level, optname, optval, optlen);
#ifdef CONFIG_NETFILTER
	 
	if (err == -ENOPROTOOPT && optname != IP_HDRINCL &&
			optname != IP_IPSEC_POLICY &&
			optname != IP_XFRM_POLICY &&
			!ip_mroute_opt(optname)) {
		lock_sock(sk);
		err = compat_nf_setsockopt(sk, PF_INET, optname,
					   optval, optlen);
		release_sock(sk);
	}
#endif
	return err;
}