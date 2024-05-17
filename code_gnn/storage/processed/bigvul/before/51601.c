void tcp_resume_early_retransmit(struct sock *sk)
{
	struct tcp_sock *tp = tcp_sk(sk);

	tcp_rearm_rto(sk);

	 
	if (!tp->do_early_retrans)
		return;

	tcp_enter_recovery(sk, false);
	tcp_update_scoreboard(sk, 1);
	tcp_xmit_retransmit_queue(sk);
}
