static inline void skb_orphan_try(struct sk_buff *skb)
{
	struct sock *sk = skb->sk;

	if (sk && !skb_shinfo(skb)->tx_flags) {
		 
		if (!skb->rxhash)
			skb->rxhash = sk->sk_hash;
		skb_orphan(skb);
	}
}
