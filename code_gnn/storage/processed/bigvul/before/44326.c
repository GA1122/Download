static int ip_route_input_slow(struct sk_buff *skb, __be32 daddr, __be32 saddr,
			       u8 tos, struct net_device *dev)
{
	struct fib_result res;
	struct in_device *in_dev = __in_dev_get_rcu(dev);
	struct flowi4	fl4;
	unsigned int	flags = 0;
	u32		itag = 0;
	struct rtable	*rth;
	int		err = -EINVAL;
	struct net    *net = dev_net(dev);
	bool do_cache;

	 

	if (!in_dev)
		goto out;

	 

	if (ipv4_is_multicast(saddr) || ipv4_is_lbcast(saddr))
		goto martian_source;

	res.fi = NULL;
	if (ipv4_is_lbcast(daddr) || (saddr == 0 && daddr == 0))
		goto brd_input;

	 
	if (ipv4_is_zeronet(saddr))
		goto martian_source;

	if (ipv4_is_zeronet(daddr))
		goto martian_destination;

	 
	if (ipv4_is_loopback(daddr)) {
		if (!IN_DEV_NET_ROUTE_LOCALNET(in_dev, net))
			goto martian_destination;
	} else if (ipv4_is_loopback(saddr)) {
		if (!IN_DEV_NET_ROUTE_LOCALNET(in_dev, net))
			goto martian_source;
	}

	 
	fl4.flowi4_oif = 0;
	fl4.flowi4_iif = dev->ifindex;
	fl4.flowi4_mark = skb->mark;
	fl4.flowi4_tos = tos;
	fl4.flowi4_scope = RT_SCOPE_UNIVERSE;
	fl4.daddr = daddr;
	fl4.saddr = saddr;
	err = fib_lookup(net, &fl4, &res);
	if (err != 0) {
		if (!IN_DEV_FORWARD(in_dev))
			err = -EHOSTUNREACH;
		goto no_route;
	}

	if (res.type == RTN_BROADCAST)
		goto brd_input;

	if (res.type == RTN_LOCAL) {
		err = fib_validate_source(skb, saddr, daddr, tos,
					  0, dev, in_dev, &itag);
		if (err < 0)
			goto martian_source_keep_err;
		goto local_input;
	}

	if (!IN_DEV_FORWARD(in_dev)) {
		err = -EHOSTUNREACH;
		goto no_route;
	}
	if (res.type != RTN_UNICAST)
		goto martian_destination;

	err = ip_mkroute_input(skb, &res, &fl4, in_dev, daddr, saddr, tos);
out:	return err;

brd_input:
	if (skb->protocol != htons(ETH_P_IP))
		goto e_inval;

	if (!ipv4_is_zeronet(saddr)) {
		err = fib_validate_source(skb, saddr, 0, tos, 0, dev,
					  in_dev, &itag);
		if (err < 0)
			goto martian_source_keep_err;
	}
	flags |= RTCF_BROADCAST;
	res.type = RTN_BROADCAST;
	RT_CACHE_STAT_INC(in_brd);

local_input:
	do_cache = false;
	if (res.fi) {
		if (!itag) {
			rth = rcu_dereference(FIB_RES_NH(res).nh_rth_input);
			if (rt_cache_valid(rth)) {
				skb_dst_set_noref(skb, &rth->dst);
				err = 0;
				goto out;
			}
			do_cache = true;
		}
	}

	rth = rt_dst_alloc(net->loopback_dev,
			   IN_DEV_CONF_GET(in_dev, NOPOLICY), false, do_cache);
	if (!rth)
		goto e_nobufs;

	rth->dst.input= ip_local_deliver;
	rth->dst.output= ip_rt_bug;
#ifdef CONFIG_IP_ROUTE_CLASSID
	rth->dst.tclassid = itag;
#endif

	rth->rt_genid = rt_genid_ipv4(net);
	rth->rt_flags 	= flags|RTCF_LOCAL;
	rth->rt_type	= res.type;
	rth->rt_is_input = 1;
	rth->rt_iif	= 0;
	rth->rt_pmtu	= 0;
	rth->rt_gateway	= 0;
	rth->rt_uses_gateway = 0;
	INIT_LIST_HEAD(&rth->rt_uncached);
	RT_CACHE_STAT_INC(in_slow_tot);
	if (res.type == RTN_UNREACHABLE) {
		rth->dst.input= ip_error;
		rth->dst.error= -err;
		rth->rt_flags 	&= ~RTCF_LOCAL;
	}
	if (do_cache) {
		if (unlikely(!rt_cache_route(&FIB_RES_NH(res), rth))) {
			rth->dst.flags |= DST_NOCACHE;
			rt_add_uncached_list(rth);
		}
	}
	skb_dst_set(skb, &rth->dst);
	err = 0;
	goto out;

no_route:
	RT_CACHE_STAT_INC(in_no_route);
	res.type = RTN_UNREACHABLE;
	res.fi = NULL;
	goto local_input;

	 
martian_destination:
	RT_CACHE_STAT_INC(in_martian_dst);
#ifdef CONFIG_IP_ROUTE_VERBOSE
	if (IN_DEV_LOG_MARTIANS(in_dev))
		net_warn_ratelimited("martian destination %pI4 from %pI4, dev %s\n",
				     &daddr, &saddr, dev->name);
#endif

e_inval:
	err = -EINVAL;
	goto out;

e_nobufs:
	err = -ENOBUFS;
	goto out;

martian_source:
	err = -EINVAL;
martian_source_keep_err:
	ip_handle_martian_source(dev, in_dev, skb, daddr, saddr);
	goto out;
}