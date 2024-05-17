static int __dev_open(struct net_device *dev)
{
	const struct net_device_ops *ops = dev->netdev_ops;
	int ret;

	ASSERT_RTNL();

	 
	if (!netif_device_present(dev))
		return -ENODEV;

	ret = call_netdevice_notifiers(NETDEV_PRE_UP, dev);
	ret = notifier_to_errno(ret);
	if (ret)
		return ret;

	 
	set_bit(__LINK_STATE_START, &dev->state);

	if (ops->ndo_validate_addr)
		ret = ops->ndo_validate_addr(dev);

	if (!ret && ops->ndo_open)
		ret = ops->ndo_open(dev);

	 

	if (ret)
		clear_bit(__LINK_STATE_START, &dev->state);
	else {
		 
		dev->flags |= IFF_UP;

		 
		net_dmaengine_get();

		 
		dev_set_rx_mode(dev);

		 
		dev_activate(dev);
	}

	return ret;
}
