static int ipx_shutdown(struct socket *sock, int mode)
{
	struct sock *sk = sock->sk;

	if (mode < SHUT_RD || mode > SHUT_RDWR)
		return -EINVAL;
	 
	++mode;

	lock_sock(sk);
	sk->sk_shutdown |= mode;
	release_sock(sk);
	sk->sk_state_change(sk);

	return 0;
}
