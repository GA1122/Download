static void ipv4_dst_destroy(struct dst_entry *dst)
{
	struct rtable *rt = (struct rtable *)dst;

	ip_dst_metrics_put(dst);
	rt_del_uncached_list(rt);
}
