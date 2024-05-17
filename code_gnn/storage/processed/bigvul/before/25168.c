static struct rtable *rt_dst_alloc(struct net_device *dev,
				   bool nopolicy, bool noxfrm)
{
	return dst_alloc(&ipv4_dst_ops, dev, 1, -1,
			 DST_HOST |
			 (nopolicy ? DST_NOPOLICY : 0) |
			 (noxfrm ? DST_NOXFRM : 0));
}
