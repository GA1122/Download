void __dev_set_rx_mode(struct net_device *dev)
{
	const struct net_device_ops *ops = dev->netdev_ops;

	 
	if (!(dev->flags&IFF_UP))
		return;

	if (!netif_device_present(dev))
		return;

	if (ops->ndo_set_rx_mode)
		ops->ndo_set_rx_mode(dev);
	else {
		 
		if (!netdev_uc_empty(dev) && !dev->uc_promisc) {
			__dev_set_promiscuity(dev, 1);
			dev->uc_promisc = 1;
		} else if (netdev_uc_empty(dev) && dev->uc_promisc) {
			__dev_set_promiscuity(dev, -1);
			dev->uc_promisc = 0;
		}

		if (ops->ndo_set_multicast_list)
			ops->ndo_set_multicast_list(dev);
	}
}