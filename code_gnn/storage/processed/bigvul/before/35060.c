SCTP_STATIC int sctp_stream_listen(struct sock *sk, int backlog)
{
	struct sctp_sock *sp = sctp_sk(sk);
	struct sctp_endpoint *ep = sp->ep;

	 
	if (!backlog) {
		if (sctp_sstate(sk, CLOSED))
			return 0;

		sctp_unhash_endpoint(ep);
		sk->sk_state = SCTP_SS_CLOSED;
	}

	if (sctp_sstate(sk, LISTENING))
		return 0;

	 
	if (!ep->base.bind_addr.port) {
		if (sctp_autobind(sk))
			return -EAGAIN;
	}
	sk->sk_state = SCTP_SS_LISTENING;
	sk->sk_max_ack_backlog = backlog;
	sctp_hash_endpoint(ep);
	return 0;
}
