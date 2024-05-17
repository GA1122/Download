int tcp_use_frto(struct sock *sk)
{
	const struct tcp_sock *tp = tcp_sk(sk);
	const struct inet_connection_sock *icsk = inet_csk(sk);
	struct sk_buff *skb;

	if (!sysctl_tcp_frto)
		return 0;

	 
	if (icsk->icsk_mtup.probe_size)
		return 0;

	if (tcp_is_sackfrto(tp))
		return 1;

	 
	if (tp->retrans_out > 1)
		return 0;

	skb = tcp_write_queue_head(sk);
	if (tcp_skb_is_last(sk, skb))
		return 1;
	skb = tcp_write_queue_next(sk, skb);	 
	tcp_for_write_queue_from(skb, sk) {
		if (skb == tcp_send_head(sk))
			break;
		if (TCP_SKB_CB(skb)->sacked & TCPCB_RETRANS)
			return 0;
		 
		if (!(TCP_SKB_CB(skb)->sacked & TCPCB_SACKED_ACKED))
			break;
	}
	return 1;
}