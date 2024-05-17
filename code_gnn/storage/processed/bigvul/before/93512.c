int ip6_mr_input(struct sk_buff *skb)
{
	struct mfc6_cache *cache;
	struct net *net = dev_net(skb->dev);
	struct mr6_table *mrt;
	struct flowi6 fl6 = {
		.flowi6_iif	= skb->dev->ifindex,
		.flowi6_mark	= skb->mark,
	};
	int err;

	err = ip6mr_fib_lookup(net, &fl6, &mrt);
	if (err < 0) {
		kfree_skb(skb);
		return err;
	}

	read_lock(&mrt_lock);
	cache = ip6mr_cache_find(mrt,
				 &ipv6_hdr(skb)->saddr, &ipv6_hdr(skb)->daddr);
	if (!cache) {
		int vif = ip6mr_find_vif(mrt, skb->dev);

		if (vif >= 0)
			cache = ip6mr_cache_find_any(mrt,
						     &ipv6_hdr(skb)->daddr,
						     vif);
	}

	 
	if (!cache) {
		int vif;

		vif = ip6mr_find_vif(mrt, skb->dev);
		if (vif >= 0) {
			int err = ip6mr_cache_unresolved(mrt, vif, skb);
			read_unlock(&mrt_lock);

			return err;
		}
		read_unlock(&mrt_lock);
		kfree_skb(skb);
		return -ENODEV;
	}

	ip6_mr_forward(net, mrt, skb, cache);

	read_unlock(&mrt_lock);

	return 0;
}