static int ip_route_input_mc(struct sk_buff *skb, __be32 daddr, __be32 saddr,
			     u8 tos, struct net_device *dev, int our)
{
	struct in_device *in_dev = __in_dev_get_rcu(dev);
	unsigned int flags = RTCF_MULTICAST;
	struct rtable *rth;
	u32 itag = 0;
	int err;

	err = ip_mc_validate_source(skb, daddr, saddr, tos, dev, in_dev, &itag);
	if (err)
		return err;

	if (our)
		flags |= RTCF_LOCAL;

	rth = rt_dst_alloc(dev_net(dev)->loopback_dev, flags, RTN_MULTICAST,
			   IN_DEV_CONF_GET(in_dev, NOPOLICY), false, false);
	if (!rth)
		return -ENOBUFS;

#ifdef CONFIG_IP_ROUTE_CLASSID
	rth->dst.tclassid = itag;
#endif
	rth->dst.output = ip_rt_bug;
	rth->rt_is_input= 1;

#ifdef CONFIG_IP_MROUTE
	if (!ipv4_is_local_multicast(daddr) && IN_DEV_MFORWARD(in_dev))
		rth->dst.input = ip_mr_input;
#endif
	RT_CACHE_STAT_INC(in_slow_mc);

	skb_dst_set(skb, &rth->dst);
	return 0;
}
