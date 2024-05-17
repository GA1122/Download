static struct sk_buff **ip4ip6_gro_receive(struct sk_buff **head,
					   struct sk_buff *skb)
{
	 

	if (NAPI_GRO_CB(skb)->encap_mark) {
		NAPI_GRO_CB(skb)->flush = 1;
		return NULL;
	}

	NAPI_GRO_CB(skb)->encap_mark = 1;

	return inet_gro_receive(head, skb);
}