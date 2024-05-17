static void tcp_ack_no_tstamp(struct sock *sk, u32 seq_rtt, int flag)
{
	 

	if (flag & FLAG_RETRANS_DATA_ACKED)
		return;

	tcp_valid_rtt_meas(sk, seq_rtt);
}
