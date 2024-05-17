static struct neighbour *ipv4_neigh_lookup(const struct dst_entry *dst, const void *daddr)
{
	struct neigh_table *tbl = &arp_tbl;
	static const __be32 inaddr_any = 0;
	struct net_device *dev = dst->dev;
	const __be32 *pkey = daddr;
	struct neighbour *n;

#if defined(CONFIG_ATM_CLIP) || defined(CONFIG_ATM_CLIP_MODULE)
	if (dev->type == ARPHRD_ATM)
		tbl = clip_tbl_hook;
#endif
	if (dev->flags & (IFF_LOOPBACK | IFF_POINTOPOINT))
		pkey = &inaddr_any;

	n = __ipv4_neigh_lookup(tbl, dev, *(__force u32 *)pkey);
	if (n)
		return n;
	return neigh_create(tbl, pkey, dev);
}
