int compat_ipv6_getsockopt(struct sock *sk, int level, int optname,
			   char __user *optval, int __user *optlen)
{
	int err;

	if (level == SOL_IP && sk->sk_type != SOCK_RAW) {
		if (udp_prot.compat_getsockopt != NULL)
			return udp_prot.compat_getsockopt(sk, level, optname,
							  optval, optlen);
		return udp_prot.getsockopt(sk, level, optname, optval, optlen);
	}

	if (level != SOL_IPV6)
		return -ENOPROTOOPT;

	if (optname == MCAST_MSFILTER)
		return compat_mc_getsockopt(sk, level, optname, optval, optlen,
			ipv6_getsockopt);

	err = do_ipv6_getsockopt(sk, level, optname, optval, optlen,
				 MSG_CMSG_COMPAT);
#ifdef CONFIG_NETFILTER
	 
	if (err == -ENOPROTOOPT && optname != IPV6_2292PKTOPTIONS) {
		int len;

		if (get_user(len, optlen))
			return -EFAULT;

		lock_sock(sk);
		err = compat_nf_getsockopt(sk, PF_INET6,
					   optname, optval, &len);
		release_sock(sk);
		if (err >= 0)
			err = put_user(len, optlen);
	}
#endif
	return err;
}