static int ipx_create(struct net *net, struct socket *sock, int protocol,
		      int kern)
{
	int rc = -ESOCKTNOSUPPORT;
	struct sock *sk;

	if (!net_eq(net, &init_net))
		return -EAFNOSUPPORT;

	 
	if (sock->type != SOCK_DGRAM)
		goto out;

	rc = -ENOMEM;
	sk = sk_alloc(net, PF_IPX, GFP_KERNEL, &ipx_proto, kern);
	if (!sk)
		goto out;

	sk_refcnt_debug_inc(sk);
	sock_init_data(sock, sk);
	sk->sk_no_check_tx = 1;		 
	sock->ops = &ipx_dgram_ops;
	rc = 0;
out:
	return rc;
}
