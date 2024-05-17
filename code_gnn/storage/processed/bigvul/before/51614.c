static bool tcp_should_expand_sndbuf(const struct sock *sk)
{
	const struct tcp_sock *tp = tcp_sk(sk);

	 
	if (sk->sk_userlocks & SOCK_SNDBUF_LOCK)
		return false;

	 
	if (tcp_under_memory_pressure(sk))
		return false;

	 
	if (sk_memory_allocated(sk) >= sk_prot_mem_limits(sk, 0))
		return false;

	 
	if (tcp_packets_in_flight(tp) >= tp->snd_cwnd)
		return false;

	return true;
}
