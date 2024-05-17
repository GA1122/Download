static u32 *ipv4_cow_metrics(struct dst_entry *dst, unsigned long old)
{
	struct rtable *rt = (struct rtable *) dst;
	struct inet_peer *peer;
	u32 *p = NULL;

	if (!rt->peer)
		rt_bind_peer(rt, rt->rt_dst, 1);

	peer = rt->peer;
	if (peer) {
		u32 *old_p = __DST_METRICS_PTR(old);
		unsigned long prev, new;

		p = peer->metrics;
		if (inet_metrics_new(peer))
			memcpy(p, old_p, sizeof(u32) * RTAX_MAX);

		new = (unsigned long) p;
		prev = cmpxchg(&dst->_metrics, old, new);

		if (prev != old) {
			p = __DST_METRICS_PTR(prev);
			if (prev & DST_METRICS_READ_ONLY)
				p = NULL;
		} else {
			if (rt->fi) {
				fib_info_put(rt->fi);
				rt->fi = NULL;
			}
		}
	}
	return p;
}
