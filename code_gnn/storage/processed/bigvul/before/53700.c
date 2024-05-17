int ipv6_setsockopt(struct sock *sk, int level, int optname,
		    char __user *optval, unsigned int optlen)
{
	int err;

	if (level == SOL_IP && sk->sk_type != SOCK_RAW)
		return udp_prot.setsockopt(sk, level, optname, optval, optlen);

	if (level != SOL_IPV6)
		return -ENOPROTOOPT;

	err = do_ipv6_setsockopt(sk, level, optname, optval, optlen);
#ifdef CONFIG_NETFILTER
	 
	if (err == -ENOPROTOOPT && optname != IPV6_IPSEC_POLICY &&
			optname != IPV6_XFRM_POLICY) {
		lock_sock(sk);
		err = nf_setsockopt(sk, PF_INET6, optname, optval,
				optlen);
		release_sock(sk);
	}
#endif
	return err;
}