int netdev_walk_all_lower_dev(struct net_device *dev,
			      int (*fn)(struct net_device *dev,
					void *data),
			      void *data)
{
	struct net_device *ldev;
	struct list_head *iter;
	int ret;

	for (iter = &dev->adj_list.lower,
	     ldev = netdev_next_lower_dev(dev, &iter);
	     ldev;
	     ldev = netdev_next_lower_dev(dev, &iter)) {
		 
		ret = fn(ldev, data);
		if (ret)
			return ret;

		 
		ret = netdev_walk_all_lower_dev(ldev, fn, data);
		if (ret)
			return ret;
	}

	return 0;
}
