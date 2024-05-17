int inet6_release(struct socket *sock)
{
	struct sock *sk = sock->sk;

	if (!sk)
		return -EINVAL;

	 
	ipv6_sock_mc_close(sk);

	 
	ipv6_sock_ac_close(sk);

	return inet_release(sock);
}
