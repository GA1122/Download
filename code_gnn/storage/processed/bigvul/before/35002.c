SCTP_STATIC int sctp_do_peeloff(struct sctp_association *asoc,
				struct socket **sockp)
{
	struct sock *sk = asoc->base.sk;
	struct socket *sock;
	struct inet_sock *inetsk;
	int err = 0;

	 
	if (!sctp_style(sk, UDP))
		return -EINVAL;

	 
	err = sock_create(sk->sk_family, SOCK_SEQPACKET, IPPROTO_SCTP, &sock);
	if (err < 0)
		return err;

	 
	sctp_sock_migrate(sk, sock->sk, asoc, SCTP_SOCKET_UDP_HIGH_BANDWIDTH);

	 
	inetsk = inet_sk(sock->sk);
	inetsk->daddr = asoc->peer.primary_addr.v4.sin_addr.s_addr;
	inetsk->id = asoc->next_tsn ^ jiffies;

	*sockp = sock;

	return err;
}
