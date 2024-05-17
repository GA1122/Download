static int tcp_should_expand_sndbuf(const struct sock *sk)
{
	const struct tcp_sock *tp = tcp_sk(sk);

	 
	if (sk->sk_userlocks & SOCK_SNDBUF_LOCK)
		return 0;

	 
	if (tcp_memory_pressure)
		return 0;

	 
	if (atomic_long_read(&tcp_memory_allocated) >= sysctl_tcp_mem[0])
		return 0;

	 
	if (tp->packets_out >= tp->snd_cwnd)
		return 0;

	return 1;
}
