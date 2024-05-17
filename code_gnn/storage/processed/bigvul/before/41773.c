static int addrconf_ifid_arcnet(u8 *eui, struct net_device *dev)
{
	 
	if (dev->addr_len != ARCNET_ALEN)
		return -1;
	memset(eui, 0, 7);
	eui[7] = *(u8 *)dev->dev_addr;
	return 0;
}