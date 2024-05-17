static void dev_addr_flush(struct net_device *dev)
{
	 

	__hw_addr_flush(&dev->dev_addrs);
	dev->dev_addr = NULL;
}
