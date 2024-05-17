static void tcp_update_pacing_rate(struct sock *sk)
{
	const struct tcp_sock *tp = tcp_sk(sk);
	u64 rate;

	 
	rate = (u64)tp->mss_cache * ((USEC_PER_SEC / 100) << 3);

	 
	if (tp->snd_cwnd < tp->snd_ssthresh / 2)
		rate *= sysctl_tcp_pacing_ss_ratio;
	else
		rate *= sysctl_tcp_pacing_ca_ratio;

	rate *= max(tp->snd_cwnd, tp->packets_out);

	if (likely(tp->srtt_us))
		do_div(rate, tp->srtt_us);

	 
	ACCESS_ONCE(sk->sk_pacing_rate) = min_t(u64, rate,
						sk->sk_max_pacing_rate);
}
