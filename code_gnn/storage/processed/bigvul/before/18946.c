static __inline__ int icmp_filter(struct sock *sk, struct sk_buff *skb)
{
	int type;

	if (!pskb_may_pull(skb, sizeof(struct icmphdr)))
		return 1;

	type = icmp_hdr(skb)->type;
	if (type < 32) {
		__u32 data = raw_sk(sk)->filter.data;

		return ((1 << type) & data) != 0;
	}

	 
	return 0;
}