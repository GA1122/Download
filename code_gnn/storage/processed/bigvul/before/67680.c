void napi_consume_skb(struct sk_buff *skb, int budget)
{
	if (unlikely(!skb))
		return;

	 
	if (unlikely(!budget)) {
		dev_consume_skb_any(skb);
		return;
	}

	if (likely(atomic_read(&skb->users) == 1))
		smp_rmb();
	else if (likely(!atomic_dec_and_test(&skb->users)))
		return;
	 
	trace_consume_skb(skb);

	 
	if (skb->fclone != SKB_FCLONE_UNAVAILABLE) {
		__kfree_skb(skb);
		return;
	}

	_kfree_skb_defer(skb);
}