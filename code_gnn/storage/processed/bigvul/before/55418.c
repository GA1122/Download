static bool tcp_try_undo_recovery(struct sock *sk)
{
	struct tcp_sock *tp = tcp_sk(sk);

	if (tcp_may_undo(tp)) {
		int mib_idx;

		 
		DBGUNDO(sk, inet_csk(sk)->icsk_ca_state == TCP_CA_Loss ? "loss" : "retrans");
		tcp_undo_cwnd_reduction(sk, false);
		if (inet_csk(sk)->icsk_ca_state == TCP_CA_Loss)
			mib_idx = LINUX_MIB_TCPLOSSUNDO;
		else
			mib_idx = LINUX_MIB_TCPFULLUNDO;

		NET_INC_STATS_BH(sock_net(sk), mib_idx);
	}
	if (tp->snd_una == tp->high_seq && tcp_is_reno(tp)) {
		 
		tcp_moderate_cwnd(tp);
		if (!tcp_any_retrans_done(sk))
			tp->retrans_stamp = 0;
		return true;
	}
	tcp_set_ca_state(sk, TCP_CA_Open);
	return false;
}