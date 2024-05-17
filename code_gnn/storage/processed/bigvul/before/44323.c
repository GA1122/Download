static int ip_mkroute_input(struct sk_buff *skb,
			    struct fib_result *res,
			    const struct flowi4 *fl4,
			    struct in_device *in_dev,
			    __be32 daddr, __be32 saddr, u32 tos)
{
#ifdef CONFIG_IP_ROUTE_MULTIPATH
	if (res->fi && res->fi->fib_nhs > 1)
		fib_select_multipath(res);
#endif

	 
	return __mkroute_input(skb, res, in_dev, daddr, saddr, tos);
}