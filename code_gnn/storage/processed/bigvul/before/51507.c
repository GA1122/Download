static void __tcp_ecn_check_ce(struct tcp_sock *tp, const struct sk_buff *skb)
{
	switch (TCP_SKB_CB(skb)->ip_dsfield & INET_ECN_MASK) {
	case INET_ECN_NOT_ECT:
		 
		if (tp->ecn_flags & TCP_ECN_SEEN)
			tcp_enter_quickack_mode((struct sock *)tp);
		break;
	case INET_ECN_CE:
		if (tcp_ca_needs_ecn((struct sock *)tp))
			tcp_ca_event((struct sock *)tp, CA_EVENT_ECN_IS_CE);

		if (!(tp->ecn_flags & TCP_ECN_DEMAND_CWR)) {
			 
			tcp_enter_quickack_mode((struct sock *)tp);
			tp->ecn_flags |= TCP_ECN_DEMAND_CWR;
		}
		tp->ecn_flags |= TCP_ECN_SEEN;
		break;
	default:
		if (tcp_ca_needs_ecn((struct sock *)tp))
			tcp_ca_event((struct sock *)tp, CA_EVENT_ECN_NO_CE);
		tp->ecn_flags |= TCP_ECN_SEEN;
		break;
	}
}