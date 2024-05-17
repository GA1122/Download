void skb_condense(struct sk_buff *skb)
{
	if (skb->data_len) {
		if (skb->data_len > skb->end - skb->tail ||
		    skb_cloned(skb))
			return;

		 
		__pskb_pull_tail(skb, skb->data_len);
	}
	 
	skb->truesize = SKB_TRUESIZE(skb_end_offset(skb));
}
