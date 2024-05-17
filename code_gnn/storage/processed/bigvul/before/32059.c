static int __dev_close(struct net_device *dev)
{
	const struct net_device_ops *ops = dev->netdev_ops;

	ASSERT_RTNL();
	might_sleep();

	 
	call_netdevice_notifiers(NETDEV_GOING_DOWN, dev);

	clear_bit(__LINK_STATE_START, &dev->state);

	 
	smp_mb__after_clear_bit();  

	dev_deactivate(dev);

	 
	if (ops->ndo_stop)
		ops->ndo_stop(dev);

	 

	dev->flags &= ~IFF_UP;

	 
	net_dmaengine_put();

	return 0;
}
