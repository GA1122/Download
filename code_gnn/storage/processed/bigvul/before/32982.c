int sctp_do_peeloff(struct sock *sk, sctp_assoc_t id, struct socket **sockp)
{
	struct sctp_association *asoc = sctp_id2assoc(sk, id);
	struct socket *sock;
	struct sctp_af *af;
	int err = 0;

	if (!asoc)
		return -EINVAL;

	 
	if (!sctp_style(sk, UDP))
		return -EINVAL;

	 
	err = sock_create(sk->sk_family, SOCK_SEQPACKET, IPPROTO_SCTP, &sock);
	if (err < 0)
		return err;

	sctp_copy_sock(sock->sk, sk, asoc);

	 
	af = sctp_get_af_specific(asoc->peer.primary_addr.sa.sa_family);
	af->to_sk_daddr(&asoc->peer.primary_addr, sk);

	 
	sctp_sock_migrate(sk, sock->sk, asoc, SCTP_SOCKET_UDP_HIGH_BANDWIDTH);

	*sockp = sock;

	return err;
}
