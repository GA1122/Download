static inline void tcp_mark_push(struct tcp_sock *tp, struct sk_buff *skb)
{
	TCP_SKB_CB(skb)->tcp_flags |= TCPHDR_PSH;
	tp->pushed_seq = tp->write_seq;
}