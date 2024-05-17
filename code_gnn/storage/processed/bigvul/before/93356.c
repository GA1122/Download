int __netdev_update_features(struct net_device *dev)
{
	struct net_device *upper, *lower;
	netdev_features_t features;
	struct list_head *iter;
	int err = -1;

	ASSERT_RTNL();

	features = netdev_get_wanted_features(dev);

	if (dev->netdev_ops->ndo_fix_features)
		features = dev->netdev_ops->ndo_fix_features(dev, features);

	 
	features = netdev_fix_features(dev, features);

	 
	netdev_for_each_upper_dev_rcu(dev, upper, iter)
		features = netdev_sync_upper_features(dev, upper, features);

	if (dev->features == features)
		goto sync_lower;

	netdev_dbg(dev, "Features changed: %pNF -> %pNF\n",
		&dev->features, &features);

	if (dev->netdev_ops->ndo_set_features)
		err = dev->netdev_ops->ndo_set_features(dev, features);
	else
		err = 0;

	if (unlikely(err < 0)) {
		netdev_err(dev,
			"set_features() failed (%d); wanted %pNF, left %pNF\n",
			err, &features, &dev->features);
		 
		return -1;
	}

sync_lower:
	 
	netdev_for_each_lower_dev(dev, lower, iter)
		netdev_sync_lower_features(dev, lower, features);

	if (!err) {
		netdev_features_t diff = features ^ dev->features;

		if (diff & NETIF_F_RX_UDP_TUNNEL_PORT) {
			 
			if (features & NETIF_F_RX_UDP_TUNNEL_PORT) {
				dev->features = features;
				udp_tunnel_get_rx_info(dev);
			} else {
				udp_tunnel_drop_rx_info(dev);
			}
		}

		dev->features = features;
	}

	return err < 0 ? 0 : 1;
}
