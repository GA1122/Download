static void tcp_disable_fack(struct tcp_sock *tp)
{
	 
	if (tcp_is_fack(tp))
		tp->lost_skb_hint = NULL;
	tp->rx_opt.sack_ok &= ~2;
}
