void skb_set_owner_w(struct sk_buff *skb, struct sock *sk)
{
	skb_orphan(skb);
	skb->sk = sk;
#ifdef CONFIG_INET
	if (unlikely(!sk_fullsock(sk))) {
		skb->destructor = sock_edemux;
		sock_hold(sk);
		return;
	}
#endif
	skb->destructor = sock_wfree;
	skb_set_hash_from_sk(skb, sk);
	 
	atomic_add(skb->truesize, &sk->sk_wmem_alloc);
}
