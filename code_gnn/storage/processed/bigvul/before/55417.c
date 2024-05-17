static bool tcp_try_undo_partial(struct sock *sk, const int acked,
				 const int prior_unsacked, int flag)
{
	struct tcp_sock *tp = tcp_sk(sk);

	if (tp->undo_marker && tcp_packet_delayed(tp)) {
		 
		tcp_update_reordering(sk, tcp_fackets_out(tp) + acked, 1);

		 
		if (tp->retrans_out) {
			tcp_cwnd_reduction(sk, prior_unsacked, 0, flag);
			return true;
		}

		if (!tcp_any_retrans_done(sk))
			tp->retrans_stamp = 0;

		DBGUNDO(sk, "partial recovery");
		tcp_undo_cwnd_reduction(sk, true);
		NET_INC_STATS_BH(sock_net(sk), LINUX_MIB_TCPPARTIALUNDO);
		tcp_try_keep_open(sk);
		return true;
	}
	return false;
}
