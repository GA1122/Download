static inline bool tcp_ack_update_rtt(struct sock *sk, const int flag,
				      long seq_rtt_us, long sack_rtt_us,
				      long ca_rtt_us)
{
	const struct tcp_sock *tp = tcp_sk(sk);

	 
	if (seq_rtt_us < 0)
		seq_rtt_us = sack_rtt_us;

	 
	if (seq_rtt_us < 0 && tp->rx_opt.saw_tstamp && tp->rx_opt.rcv_tsecr &&
	    flag & FLAG_ACKED)
		seq_rtt_us = ca_rtt_us = jiffies_to_usecs(tcp_time_stamp -
							  tp->rx_opt.rcv_tsecr);
	if (seq_rtt_us < 0)
		return false;

	 
	tcp_update_rtt_min(sk, ca_rtt_us);
	tcp_rtt_estimator(sk, seq_rtt_us);
	tcp_set_rto(sk);

	 
	inet_csk(sk)->icsk_backoff = 0;
	return true;
}
