static int __dev_close_many(struct list_head *head)
{
	struct net_device *dev;

	ASSERT_RTNL();
	might_sleep();

	list_for_each_entry(dev, head, unreg_list) {
		 
		call_netdevice_notifiers(NETDEV_GOING_DOWN, dev);

		clear_bit(__LINK_STATE_START, &dev->state);

		 
		smp_mb__after_clear_bit();  
	}

	dev_deactivate_many(head);

	list_for_each_entry(dev, head, unreg_list) {
		const struct net_device_ops *ops = dev->netdev_ops;

		 
		if (ops->ndo_stop)
			ops->ndo_stop(dev);

		 

		dev->flags &= ~IFF_UP;

		 
		net_dmaengine_put();
	}

	return 0;
}
