static void tcp_ack_probe(struct sock *sk)
{
	const struct tcp_sock *tp = tcp_sk(sk);
	struct inet_connection_sock *icsk = inet_csk(sk);

	 

	if (!after(TCP_SKB_CB(tcp_send_head(sk))->end_seq, tcp_wnd_end(tp))) {
		icsk->icsk_backoff = 0;
		inet_csk_clear_xmit_timer(sk, ICSK_TIME_PROBE0);
		 
	} else {
		unsigned long when = tcp_probe0_when(sk, TCP_RTO_MAX);

		inet_csk_reset_xmit_timer(sk, ICSK_TIME_PROBE0,
					  when, TCP_RTO_MAX);
	}
}