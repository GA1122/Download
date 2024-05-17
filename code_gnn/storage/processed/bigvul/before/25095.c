struct rtable *__ip_route_output_key(struct net *net, struct flowi4 *flp4)
{
	struct rtable *rth;
	unsigned int hash;

	if (!rt_caching(net))
		goto slow_output;

	hash = rt_hash(flp4->daddr, flp4->saddr, flp4->flowi4_oif, rt_genid(net));

	rcu_read_lock_bh();
	for (rth = rcu_dereference_bh(rt_hash_table[hash].chain); rth;
		rth = rcu_dereference_bh(rth->dst.rt_next)) {
		if (rth->rt_key_dst == flp4->daddr &&
		    rth->rt_key_src == flp4->saddr &&
		    rt_is_output_route(rth) &&
		    rth->rt_oif == flp4->flowi4_oif &&
		    rth->rt_mark == flp4->flowi4_mark &&
		    !((rth->rt_key_tos ^ flp4->flowi4_tos) &
			    (IPTOS_RT_MASK | RTO_ONLINK)) &&
		    net_eq(dev_net(rth->dst.dev), net) &&
		    !rt_is_expired(rth)) {
			dst_use(&rth->dst, jiffies);
			RT_CACHE_STAT_INC(out_hit);
			rcu_read_unlock_bh();
			if (!flp4->saddr)
				flp4->saddr = rth->rt_src;
			if (!flp4->daddr)
				flp4->daddr = rth->rt_dst;
			return rth;
		}
		RT_CACHE_STAT_INC(out_hlist_search);
	}
	rcu_read_unlock_bh();

slow_output:
	return ip_route_output_slow(net, flp4);
}
