static void rt_init_metrics(struct rtable *rt, const struct flowi4 *fl4,
			    struct fib_info *fi)
{
	struct inet_peer *peer;
	int create = 0;

	 
	if (fl4 && (fl4->flowi4_flags & FLOWI_FLAG_PRECOW_METRICS))
		create = 1;

	rt->peer = peer = inet_getpeer_v4(rt->rt_dst, create);
	if (peer) {
		rt->rt_peer_genid = rt_peer_genid();
		if (inet_metrics_new(peer))
			memcpy(peer->metrics, fi->fib_metrics,
			       sizeof(u32) * RTAX_MAX);
		dst_init_metrics(&rt->dst, peer->metrics, false);

		check_peer_pmtu(&rt->dst, peer);
		if (peer->redirect_learned.a4 &&
		    peer->redirect_learned.a4 != rt->rt_gateway) {
			rt->rt_gateway = peer->redirect_learned.a4;
			rt->rt_flags |= RTCF_REDIRECTED;
		}
	} else {
		if (fi->fib_metrics != (u32 *) dst_default_metrics) {
			rt->fi = fi;
			atomic_inc(&fi->fib_clntref);
		}
		dst_init_metrics(&rt->dst, fi->fib_metrics, true);
	}
}
