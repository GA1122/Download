static struct dst_entry *ipv4_negative_advice(struct dst_entry *dst)
{
	struct rtable *rt = (struct rtable *)dst;
	struct dst_entry *ret = dst;

	if (rt) {
		if (dst->obsolete > 0) {
			ip_rt_put(rt);
			ret = NULL;
		} else if (rt->rt_flags & RTCF_REDIRECTED) {
			unsigned hash = rt_hash(rt->rt_key_dst, rt->rt_key_src,
						rt->rt_oif,
						rt_genid(dev_net(dst->dev)));
			rt_del(hash, rt);
			ret = NULL;
		} else if (rt->peer && peer_pmtu_expired(rt->peer)) {
			dst_metric_set(dst, RTAX_MTU, rt->peer->pmtu_orig);
		}
	}
	return ret;
}
