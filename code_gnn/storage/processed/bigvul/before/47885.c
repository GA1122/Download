void skb_orphan_partial(struct sk_buff *skb)
{
	 
	if (skb->truesize <= 2)
		return;

	 
	if (skb->destructor == sock_wfree
#ifdef CONFIG_INET
	    || skb->destructor == tcp_wfree
#endif
		) {
		atomic_sub(skb->truesize - 1, &skb->sk->sk_wmem_alloc);
		skb->truesize = 1;
	} else {
		skb_orphan(skb);
	}
}