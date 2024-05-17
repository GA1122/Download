static int sctp_listen_start(struct sock *sk, int backlog)
{
	struct sctp_sock *sp = sctp_sk(sk);
	struct sctp_endpoint *ep = sp->ep;
	struct crypto_shash *tfm = NULL;
	char alg[32];

	 
	if (!sp->hmac && sp->sctp_hmac_alg) {
		sprintf(alg, "hmac(%s)", sp->sctp_hmac_alg);
		tfm = crypto_alloc_shash(alg, 0, 0);
		if (IS_ERR(tfm)) {
			net_info_ratelimited("failed to load transform for %s: %ld\n",
					     sp->sctp_hmac_alg, PTR_ERR(tfm));
			return -ENOSYS;
		}
		sctp_sk(sk)->hmac = tfm;
	}

	 
	sk->sk_state = SCTP_SS_LISTENING;
	if (!ep->base.bind_addr.port) {
		if (sctp_autobind(sk))
			return -EAGAIN;
	} else {
		if (sctp_get_port(sk, inet_sk(sk)->inet_num)) {
			sk->sk_state = SCTP_SS_CLOSED;
			return -EADDRINUSE;
		}
	}

	sk->sk_max_ack_backlog = backlog;
	sctp_hash_endpoint(ep);
	return 0;
}
