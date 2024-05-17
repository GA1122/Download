static void addrconf_dev_config(struct net_device *dev)
{
	struct inet6_dev *idev;

	ASSERT_RTNL();

	if ((dev->type != ARPHRD_ETHER) &&
	    (dev->type != ARPHRD_FDDI) &&
	    (dev->type != ARPHRD_ARCNET) &&
	    (dev->type != ARPHRD_INFINIBAND) &&
	    (dev->type != ARPHRD_IEEE802154) &&
	    (dev->type != ARPHRD_IEEE1394) &&
	    (dev->type != ARPHRD_TUNNEL6) &&
	    (dev->type != ARPHRD_6LOWPAN)) {
		 
		return;
	}

	idev = addrconf_add_dev(dev);
	if (IS_ERR(idev))
		return;

	addrconf_addr_gen(idev, false);
}