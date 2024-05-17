static void rt_set_nexthop(struct rtable *rt, const struct flowi4 *fl4,
			   const struct fib_result *res,
			   struct fib_info *fi, u16 type, u32 itag)
{
	struct dst_entry *dst = &rt->dst;

	if (fi) {
		if (FIB_RES_GW(*res) &&
		    FIB_RES_NH(*res).nh_scope == RT_SCOPE_LINK)
			rt->rt_gateway = FIB_RES_GW(*res);
		rt_init_metrics(rt, fl4, fi);
#ifdef CONFIG_IP_ROUTE_CLASSID
		dst->tclassid = FIB_RES_NH(*res).nh_tclassid;
#endif
	}

	if (dst_mtu(dst) > IP_MAX_MTU)
		dst_metric_set(dst, RTAX_MTU, IP_MAX_MTU);
	if (dst_metric_raw(dst, RTAX_ADVMSS) > 65535 - 40)
		dst_metric_set(dst, RTAX_ADVMSS, 65535 - 40);

#ifdef CONFIG_IP_ROUTE_CLASSID
#ifdef CONFIG_IP_MULTIPLE_TABLES
	set_class_tag(rt, fib_rules_tclass(res));
#endif
	set_class_tag(rt, itag);
#endif
}
