static int pfkey_broadcast(struct sk_buff *skb, gfp_t allocation,
			   int broadcast_flags, struct sock *one_sk,
			   struct net *net)
{
	struct netns_pfkey *net_pfkey = net_generic(net, pfkey_net_id);
	struct sock *sk;
	struct sk_buff *skb2 = NULL;
	int err = -ESRCH;

	 
	if (!skb)
		return -ENOMEM;

	rcu_read_lock();
	sk_for_each_rcu(sk, &net_pfkey->table) {
		struct pfkey_sock *pfk = pfkey_sk(sk);
		int err2;

		 
		if (pfk->promisc)
			pfkey_broadcast_one(skb, &skb2, allocation, sk);

		 
		if (sk == one_sk)
			continue;
		if (broadcast_flags != BROADCAST_ALL) {
			if (broadcast_flags & BROADCAST_PROMISC_ONLY)
				continue;
			if ((broadcast_flags & BROADCAST_REGISTERED) &&
			    !pfk->registered)
				continue;
			if (broadcast_flags & BROADCAST_ONE)
				continue;
		}

		err2 = pfkey_broadcast_one(skb, &skb2, allocation, sk);

		 
		if ((broadcast_flags & BROADCAST_REGISTERED) && err)
			err = err2;
	}
	rcu_read_unlock();

	if (one_sk != NULL)
		err = pfkey_broadcast_one(skb, &skb2, allocation, one_sk);

	kfree_skb(skb2);
	kfree_skb(skb);
	return err;
}
