static int ip_mkroute_input(struct sk_buff *skb,
			    struct fib_result *res,
			    const struct flowi4 *fl4,
			    struct in_device *in_dev,
			    __be32 daddr, __be32 saddr, u32 tos)
{
	struct rtable* rth = NULL;
	int err;
	unsigned hash;

#ifdef CONFIG_IP_ROUTE_MULTIPATH
	if (res->fi && res->fi->fib_nhs > 1)
		fib_select_multipath(res);
#endif

	 
	err = __mkroute_input(skb, res, in_dev, daddr, saddr, tos, &rth);
	if (err)
		return err;

	 
	hash = rt_hash(daddr, saddr, fl4->flowi4_iif,
		       rt_genid(dev_net(rth->dst.dev)));
	rth = rt_intern_hash(hash, rth, skb, fl4->flowi4_iif);
	if (IS_ERR(rth))
		return PTR_ERR(rth);
	return 0;
}
