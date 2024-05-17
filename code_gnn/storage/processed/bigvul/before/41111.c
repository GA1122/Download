static void tcp_ack_saw_tstamp(struct sock *sk, int flag)
{
	 
	struct tcp_sock *tp = tcp_sk(sk);

	tcp_valid_rtt_meas(sk, tcp_time_stamp - tp->rx_opt.rcv_tsecr);
}
