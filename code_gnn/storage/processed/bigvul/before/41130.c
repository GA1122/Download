static inline void tcp_complete_cwr(struct sock *sk)
{
	struct tcp_sock *tp = tcp_sk(sk);

	 
	if (tp->undo_marker) {
		if (inet_csk(sk)->icsk_ca_state == TCP_CA_CWR)
			tp->snd_cwnd = min(tp->snd_cwnd, tp->snd_ssthresh);
		else  
			tp->snd_cwnd = tp->snd_ssthresh;
		tp->snd_cwnd_stamp = tcp_time_stamp;
	}
	tcp_ca_event(sk, CA_EVENT_COMPLETE_CWR);
}