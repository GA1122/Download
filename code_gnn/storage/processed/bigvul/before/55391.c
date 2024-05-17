static inline bool tcp_may_raise_cwnd(const struct sock *sk, const int flag)
{
	if (tcp_in_cwnd_reduction(sk))
		return false;

	 
	if (tcp_sk(sk)->reordering > sysctl_tcp_reordering)
		return flag & FLAG_FORWARD_PROGRESS;

	return flag & FLAG_DATA_ACKED;
}
