static unsigned int ipv4_default_mtu(const struct dst_entry *dst)
{
	unsigned int mtu = dst->dev->mtu;

	if (unlikely(dst_metric_locked(dst, RTAX_MTU))) {
		const struct rtable *rt = (const struct rtable *) dst;

		if (rt->rt_gateway != rt->rt_dst && mtu > 576)
			mtu = 576;
	}

	if (mtu > IP_MAX_MTU)
		mtu = IP_MAX_MTU;

	return mtu;
}
