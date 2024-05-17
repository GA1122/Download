static void rt_set_nexthop(struct rtable *rt, __be32 daddr,
			   const struct fib_result *res,
			   struct fib_nh_exception *fnhe,
			   struct fib_info *fi, u16 type, u32 itag)
{
	bool cached = false;

	if (fi) {
		struct fib_nh *nh = &FIB_RES_NH(*res);

		if (nh->nh_gw && nh->nh_scope == RT_SCOPE_LINK) {
			rt->rt_gateway = nh->nh_gw;
			rt->rt_uses_gateway = 1;
		}
		dst_init_metrics(&rt->dst, fi->fib_metrics, true);
#ifdef CONFIG_IP_ROUTE_CLASSID
		rt->dst.tclassid = nh->nh_tclassid;
#endif
		if (unlikely(fnhe))
			cached = rt_bind_exception(rt, fnhe, daddr);
		else if (!(rt->dst.flags & DST_NOCACHE))
			cached = rt_cache_route(nh, rt);
		if (unlikely(!cached)) {
			 
			rt->dst.flags |= DST_NOCACHE;
			if (!rt->rt_gateway)
				rt->rt_gateway = daddr;
			rt_add_uncached_list(rt);
		}
	} else
		rt_add_uncached_list(rt);

#ifdef CONFIG_IP_ROUTE_CLASSID
#ifdef CONFIG_IP_MULTIPLE_TABLES
	set_class_tag(rt, res->tclassid);
#endif
	set_class_tag(rt, itag);
#endif
}