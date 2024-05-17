static void irda_connect_response(struct irda_sock *self)
{
	struct sk_buff *skb;

	skb = alloc_skb(TTP_MAX_HEADER + TTP_SAR_HEADER, GFP_KERNEL);
	if (skb == NULL) {
		pr_debug("%s() Unable to allocate sk_buff!\n",
			 __func__);
		return;
	}

	 
	skb_reserve(skb, IRDA_MAX_HEADER);

	irttp_connect_response(self->tsap, self->max_sdu_size_rx, skb);
}