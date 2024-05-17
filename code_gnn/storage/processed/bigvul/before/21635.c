static void ip_mc_filter_add(struct in_device *in_dev, __be32 addr)
{
	char buf[MAX_ADDR_LEN];
	struct net_device *dev = in_dev->dev;

	 
	if (arp_mc_map(addr, buf, dev, 0) == 0)
		dev_mc_add(dev, buf);
}
