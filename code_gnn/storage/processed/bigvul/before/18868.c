static inline bool icmpv4_xrlim_allow(struct net *net, struct rtable *rt,
		int type, int code)
{
	struct dst_entry *dst = &rt->dst;
	bool rc = true;

	if (type > NR_ICMP_TYPES)
		goto out;

	 
	if (type == ICMP_DEST_UNREACH && code == ICMP_FRAG_NEEDED)
		goto out;

	 
	if (dst->dev && (dst->dev->flags&IFF_LOOPBACK))
		goto out;

	 
	if ((1 << type) & net->ipv4.sysctl_icmp_ratemask) {
		if (!rt->peer)
			rt_bind_peer(rt, 1);
		rc = inet_peer_xrlim_allow(rt->peer,
					   net->ipv4.sysctl_icmp_ratelimit);
	}
out:
	return rc;
}