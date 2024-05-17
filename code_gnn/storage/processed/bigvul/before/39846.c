struct sk_buff *__alloc_skb_head(gfp_t gfp_mask, int node)
{
	struct sk_buff *skb;

	 
	skb = kmem_cache_alloc_node(skbuff_head_cache,
				    gfp_mask & ~__GFP_DMA, node);
	if (!skb)
		goto out;

	 
	memset(skb, 0, offsetof(struct sk_buff, tail));
	skb->head = NULL;
	skb->truesize = sizeof(struct sk_buff);
	atomic_set(&skb->users, 1);

	skb->mac_header = (typeof(skb->mac_header))~0U;
out:
	return skb;
}
