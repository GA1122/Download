static void kfree_skbmem(struct sk_buff *skb)
{
	struct sk_buff_fclones *fclones;

	switch (skb->fclone) {
	case SKB_FCLONE_UNAVAILABLE:
		kmem_cache_free(skbuff_head_cache, skb);
		return;

	case SKB_FCLONE_ORIG:
		fclones = container_of(skb, struct sk_buff_fclones, skb1);

		 
		if (atomic_read(&fclones->fclone_ref) == 1)
			goto fastpath;
		break;

	default:  
		fclones = container_of(skb, struct sk_buff_fclones, skb2);
		break;
	}
	if (!atomic_dec_and_test(&fclones->fclone_ref))
		return;
fastpath:
	kmem_cache_free(skbuff_fclone_cache, fclones);
}
