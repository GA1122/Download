static struct rtable *rt_intern_hash(unsigned hash, struct rtable *rt,
				     struct sk_buff *skb, int ifindex)
{
	struct rtable	*rth, *cand;
	struct rtable __rcu **rthp, **candp;
	unsigned long	now;
	u32 		min_score;
	int		chain_length;
	int attempts = !in_softirq();

restart:
	chain_length = 0;
	min_score = ~(u32)0;
	cand = NULL;
	candp = NULL;
	now = jiffies;

	if (!rt_caching(dev_net(rt->dst.dev))) {
		 

		rt->dst.flags |= DST_NOCACHE;
		if (rt->rt_type == RTN_UNICAST || rt_is_output_route(rt)) {
			int err = rt_bind_neighbour(rt);
			if (err) {
				if (net_ratelimit())
					printk(KERN_WARNING
					    "Neighbour table failure & not caching routes.\n");
				ip_rt_put(rt);
				return ERR_PTR(err);
			}
		}

		goto skip_hashing;
	}

	rthp = &rt_hash_table[hash].chain;

	spin_lock_bh(rt_hash_lock_addr(hash));
	while ((rth = rcu_dereference_protected(*rthp,
			lockdep_is_held(rt_hash_lock_addr(hash)))) != NULL) {
		if (rt_is_expired(rth)) {
			*rthp = rth->dst.rt_next;
			rt_free(rth);
			continue;
		}
		if (compare_keys(rth, rt) && compare_netns(rth, rt)) {
			 
			*rthp = rth->dst.rt_next;
			 
			rcu_assign_pointer(rth->dst.rt_next,
					   rt_hash_table[hash].chain);
			 
			rcu_assign_pointer(rt_hash_table[hash].chain, rth);

			dst_use(&rth->dst, now);
			spin_unlock_bh(rt_hash_lock_addr(hash));

			rt_drop(rt);
			if (skb)
				skb_dst_set(skb, &rth->dst);
			return rth;
		}

		if (!atomic_read(&rth->dst.__refcnt)) {
			u32 score = rt_score(rth);

			if (score <= min_score) {
				cand = rth;
				candp = rthp;
				min_score = score;
			}
		}

		chain_length++;

		rthp = &rth->dst.rt_next;
	}

	if (cand) {
		 
		if (chain_length > ip_rt_gc_elasticity) {
			*candp = cand->dst.rt_next;
			rt_free(cand);
		}
	} else {
		if (chain_length > rt_chain_length_max &&
		    slow_chain_length(rt_hash_table[hash].chain) > rt_chain_length_max) {
			struct net *net = dev_net(rt->dst.dev);
			int num = ++net->ipv4.current_rt_cache_rebuild_count;
			if (!rt_caching(net)) {
				printk(KERN_WARNING "%s: %d rebuilds is over limit, route caching disabled\n",
					rt->dst.dev->name, num);
			}
			rt_emergency_hash_rebuild(net);
			spin_unlock_bh(rt_hash_lock_addr(hash));

			hash = rt_hash(rt->rt_key_dst, rt->rt_key_src,
					ifindex, rt_genid(net));
			goto restart;
		}
	}

	 
	if (rt->rt_type == RTN_UNICAST || rt_is_output_route(rt)) {
		int err = rt_bind_neighbour(rt);
		if (err) {
			spin_unlock_bh(rt_hash_lock_addr(hash));

			if (err != -ENOBUFS) {
				rt_drop(rt);
				return ERR_PTR(err);
			}

			 
			if (attempts-- > 0) {
				int saved_elasticity = ip_rt_gc_elasticity;
				int saved_int = ip_rt_gc_min_interval;
				ip_rt_gc_elasticity	= 1;
				ip_rt_gc_min_interval	= 0;
				rt_garbage_collect(&ipv4_dst_ops);
				ip_rt_gc_min_interval	= saved_int;
				ip_rt_gc_elasticity	= saved_elasticity;
				goto restart;
			}

			if (net_ratelimit())
				printk(KERN_WARNING "ipv4: Neighbour table overflow.\n");
			rt_drop(rt);
			return ERR_PTR(-ENOBUFS);
		}
	}

	rt->dst.rt_next = rt_hash_table[hash].chain;

	 
	rcu_assign_pointer(rt_hash_table[hash].chain, rt);

	spin_unlock_bh(rt_hash_lock_addr(hash));

skip_hashing:
	if (skb)
		skb_dst_set(skb, &rt->dst);
	return rt;
}
