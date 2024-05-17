void tcp_v4_mtu_reduced(struct sock *sk)
{
	struct dst_entry *dst;
	struct inet_sock *inet = inet_sk(sk);
	u32 mtu = tcp_sk(sk)->mtu_info;

	dst = inet_csk_update_pmtu(sk, mtu);
	if (!dst)
		return;

	 
	if (mtu < dst_mtu(dst) && ip_dont_fragment(sk, dst))
		sk->sk_err_soft = EMSGSIZE;

	mtu = dst_mtu(dst);

	if (inet->pmtudisc != IP_PMTUDISC_DONT &&
	    ip_sk_accept_pmtu(sk) &&
	    inet_csk(sk)->icsk_pmtu_cookie > mtu) {
		tcp_sync_mss(sk, mtu);

		 
		tcp_simple_retransmit(sk);
	}  
}
