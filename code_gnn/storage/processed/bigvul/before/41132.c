static void tcp_conservative_spur_to_response(struct tcp_sock *tp)
{
	tp->snd_cwnd = min(tp->snd_cwnd, tp->snd_ssthresh);
	tp->snd_cwnd_cnt = 0;
	tp->bytes_acked = 0;
	TCP_ECN_queue_cwr(tp);
	tcp_moderate_cwnd(tp);
}