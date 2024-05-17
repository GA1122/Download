struct sock *inet_csk_clone(struct sock *sk, const struct request_sock *req,
			    const gfp_t priority)
{
	struct sock *newsk = sk_clone(sk, priority);

	if (newsk != NULL) {
		struct inet_connection_sock *newicsk = inet_csk(newsk);

		newsk->sk_state = TCP_SYN_RECV;
		newicsk->icsk_bind_hash = NULL;

		inet_sk(newsk)->inet_dport = inet_rsk(req)->rmt_port;
		inet_sk(newsk)->inet_num = ntohs(inet_rsk(req)->loc_port);
		inet_sk(newsk)->inet_sport = inet_rsk(req)->loc_port;
		newsk->sk_write_space = sk_stream_write_space;

		newicsk->icsk_retransmits = 0;
		newicsk->icsk_backoff	  = 0;
		newicsk->icsk_probes_out  = 0;

		 
		memset(&newicsk->icsk_accept_queue, 0, sizeof(newicsk->icsk_accept_queue));

		security_inet_csk_clone(newsk, req);
	}
	return newsk;
}