void napi_gro_flush(struct napi_struct *napi, bool flush_old)
{
	struct sk_buff *skb, *prev = NULL;

	 
	for (skb = napi->gro_list; skb != NULL; skb = skb->next) {
		skb->prev = prev;
		prev = skb;
	}

	for (skb = prev; skb; skb = prev) {
		skb->next = NULL;

		if (flush_old && NAPI_GRO_CB(skb)->age == jiffies)
			return;

		prev = skb->prev;
		napi_gro_complete(skb);
		napi->gro_count--;
	}

	napi->gro_list = NULL;
}