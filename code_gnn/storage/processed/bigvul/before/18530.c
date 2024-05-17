static void tcp_illinois_cong_avoid(struct sock *sk, u32 ack, u32 in_flight)
{
	struct tcp_sock *tp = tcp_sk(sk);
	struct illinois *ca = inet_csk_ca(sk);

	if (after(ack, ca->end_seq))
		update_params(sk);

	 
	if (!tcp_is_cwnd_limited(sk, in_flight))
		return;

	 
	if (tp->snd_cwnd <= tp->snd_ssthresh)
		tcp_slow_start(tp);

	else {
		u32 delta;

		 
		tp->snd_cwnd_cnt += ca->acked;
		ca->acked = 1;

		 
		delta = (tp->snd_cwnd_cnt * ca->alpha) >> ALPHA_SHIFT;
		if (delta >= tp->snd_cwnd) {
			tp->snd_cwnd = min(tp->snd_cwnd + delta / tp->snd_cwnd,
					   (u32) tp->snd_cwnd_clamp);
			tp->snd_cwnd_cnt = 0;
		}
	}
}