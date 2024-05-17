static int ip_route_input_mc(struct sk_buff *skb, __be32 daddr, __be32 saddr,
				u8 tos, struct net_device *dev, int our)
{
	struct rtable *rth;
	struct in_device *in_dev = __in_dev_get_rcu(dev);
	u32 itag = 0;
	int err;

	 

	if (in_dev == NULL)
		return -EINVAL;

	if (ipv4_is_multicast(saddr) || ipv4_is_lbcast(saddr) ||
	    skb->protocol != htons(ETH_P_IP))
		goto e_inval;

	if (likely(!IN_DEV_ROUTE_LOCALNET(in_dev)))
		if (ipv4_is_loopback(saddr))
			goto e_inval;

	if (ipv4_is_zeronet(saddr)) {
		if (!ipv4_is_local_multicast(daddr))
			goto e_inval;
	} else {
		err = fib_validate_source(skb, saddr, 0, tos, 0, dev,
					  in_dev, &itag);
		if (err < 0)
			goto e_err;
	}
	rth = rt_dst_alloc(dev_net(dev)->loopback_dev,
			   IN_DEV_CONF_GET(in_dev, NOPOLICY), false, false);
	if (!rth)
		goto e_nobufs;

#ifdef CONFIG_IP_ROUTE_CLASSID
	rth->dst.tclassid = itag;
#endif
	rth->dst.output = ip_rt_bug;

	rth->rt_genid	= rt_genid_ipv4(dev_net(dev));
	rth->rt_flags	= RTCF_MULTICAST;
	rth->rt_type	= RTN_MULTICAST;
	rth->rt_is_input= 1;
	rth->rt_iif	= 0;
	rth->rt_pmtu	= 0;
	rth->rt_gateway	= 0;
	rth->rt_uses_gateway = 0;
	INIT_LIST_HEAD(&rth->rt_uncached);
	if (our) {
		rth->dst.input= ip_local_deliver;
		rth->rt_flags |= RTCF_LOCAL;
	}

#ifdef CONFIG_IP_MROUTE
	if (!ipv4_is_local_multicast(daddr) && IN_DEV_MFORWARD(in_dev))
		rth->dst.input = ip_mr_input;
#endif
	RT_CACHE_STAT_INC(in_slow_mc);

	skb_dst_set(skb, &rth->dst);
	return 0;

e_nobufs:
	return -ENOBUFS;
e_inval:
	return -EINVAL;
e_err:
	return err;
}