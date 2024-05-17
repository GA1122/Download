static bool tcp_time_to_recover(struct sock *sk, int flag)
{
	struct tcp_sock *tp = tcp_sk(sk);
	__u32 packets_out;
	int tcp_reordering = sock_net(sk)->ipv4.sysctl_tcp_reordering;

	 
	if (tp->lost_out)
		return true;

	 
	if (tcp_dupack_heuristics(tp) > tp->reordering)
		return true;

	 
	packets_out = tp->packets_out;
	if (packets_out <= tp->reordering &&
	    tp->sacked_out >= max_t(__u32, packets_out/2, tcp_reordering) &&
	    !tcp_may_send_now(sk)) {
		 
		return true;
	}

	 
	if ((tp->thin_dupack || sysctl_tcp_thin_dupack) &&
	    tcp_stream_is_thin(tp) && tcp_dupack_heuristics(tp) > 1 &&
	    tcp_is_sack(tp) && !tcp_send_head(sk))
		return true;

	 
	if (tp->do_early_retrans && !tp->retrans_out && tp->sacked_out &&
	    (tp->packets_out >= (tp->sacked_out + 1) && tp->packets_out < 4) &&
	    !tcp_may_send_now(sk))
		return !tcp_pause_early_retransmit(sk, flag);

	return false;
}
