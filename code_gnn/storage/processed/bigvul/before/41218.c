static int tcp_time_to_recover(struct sock *sk)
{
	struct tcp_sock *tp = tcp_sk(sk);
	__u32 packets_out;

	 
	if (tp->frto_counter)
		return 0;

	 
	if (tp->lost_out)
		return 1;

	 
	if (tcp_dupack_heuristics(tp) > tp->reordering)
		return 1;

	 
	if (tcp_is_fack(tp) && tcp_head_timedout(sk))
		return 1;

	 
	packets_out = tp->packets_out;
	if (packets_out <= tp->reordering &&
	    tp->sacked_out >= max_t(__u32, packets_out/2, sysctl_tcp_reordering) &&
	    !tcp_may_send_now(sk)) {
		 
		return 1;
	}

	 
	if ((tp->thin_dupack || sysctl_tcp_thin_dupack) &&
	    tcp_stream_is_thin(tp) && tcp_dupack_heuristics(tp) > 1 &&
	    tcp_is_sack(tp) && !tcp_send_head(sk))
		return 1;

	return 0;
}
