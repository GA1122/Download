void hostap_remove_interface(struct net_device *dev, int rtnl_locked,
			     int remove_from_list)
{
	struct hostap_interface *iface;

	if (!dev)
		return;

	iface = netdev_priv(dev);

	if (remove_from_list) {
		list_del(&iface->list);
	}

	if (dev == iface->local->ddev)
		iface->local->ddev = NULL;
	else if (dev == iface->local->apdev)
		iface->local->apdev = NULL;
	else if (dev == iface->local->stadev)
		iface->local->stadev = NULL;

	if (rtnl_locked)
		unregister_netdevice(dev);
	else
		unregister_netdev(dev);

	 
}