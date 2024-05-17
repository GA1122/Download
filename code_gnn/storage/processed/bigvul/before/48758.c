static int __netdev_upper_dev_link(struct net_device *dev,
				   struct net_device *upper_dev, bool master,
				   void *upper_priv, void *upper_info)
{
	struct netdev_notifier_changeupper_info changeupper_info;
	struct netdev_adjacent *i, *j, *to_i, *to_j;
	int ret = 0;

	ASSERT_RTNL();

	if (dev == upper_dev)
		return -EBUSY;

	 
	if (__netdev_find_adj(dev, &upper_dev->all_adj_list.upper))
		return -EBUSY;

	if (__netdev_find_adj(upper_dev, &dev->adj_list.upper))
		return -EEXIST;

	if (master && netdev_master_upper_dev_get(dev))
		return -EBUSY;

	changeupper_info.upper_dev = upper_dev;
	changeupper_info.master = master;
	changeupper_info.linking = true;
	changeupper_info.upper_info = upper_info;

	ret = call_netdevice_notifiers_info(NETDEV_PRECHANGEUPPER, dev,
					    &changeupper_info.info);
	ret = notifier_to_errno(ret);
	if (ret)
		return ret;

	ret = __netdev_adjacent_dev_link_neighbour(dev, upper_dev, upper_priv,
						   master);
	if (ret)
		return ret;

	 
	list_for_each_entry(i, &dev->all_adj_list.lower, list) {
		list_for_each_entry(j, &upper_dev->all_adj_list.upper, list) {
			pr_debug("Interlinking %s with %s, non-neighbour\n",
				 i->dev->name, j->dev->name);
			ret = __netdev_adjacent_dev_link(i->dev, j->dev);
			if (ret)
				goto rollback_mesh;
		}
	}

	 
	list_for_each_entry(i, &upper_dev->all_adj_list.upper, list) {
		pr_debug("linking %s's upper device %s with %s\n",
			 upper_dev->name, i->dev->name, dev->name);
		ret = __netdev_adjacent_dev_link(dev, i->dev);
		if (ret)
			goto rollback_upper_mesh;
	}

	 
	list_for_each_entry(i, &dev->all_adj_list.lower, list) {
		pr_debug("linking %s's lower device %s with %s\n", dev->name,
			 i->dev->name, upper_dev->name);
		ret = __netdev_adjacent_dev_link(i->dev, upper_dev);
		if (ret)
			goto rollback_lower_mesh;
	}

	ret = call_netdevice_notifiers_info(NETDEV_CHANGEUPPER, dev,
					    &changeupper_info.info);
	ret = notifier_to_errno(ret);
	if (ret)
		goto rollback_lower_mesh;

	return 0;

rollback_lower_mesh:
	to_i = i;
	list_for_each_entry(i, &dev->all_adj_list.lower, list) {
		if (i == to_i)
			break;
		__netdev_adjacent_dev_unlink(i->dev, upper_dev);
	}

	i = NULL;

rollback_upper_mesh:
	to_i = i;
	list_for_each_entry(i, &upper_dev->all_adj_list.upper, list) {
		if (i == to_i)
			break;
		__netdev_adjacent_dev_unlink(dev, i->dev);
	}

	i = j = NULL;

rollback_mesh:
	to_i = i;
	to_j = j;
	list_for_each_entry(i, &dev->all_adj_list.lower, list) {
		list_for_each_entry(j, &upper_dev->all_adj_list.upper, list) {
			if (i == to_i && j == to_j)
				break;
			__netdev_adjacent_dev_unlink(i->dev, j->dev);
		}
		if (i == to_i)
			break;
	}

	__netdev_adjacent_dev_unlink_neighbour(dev, upper_dev);

	return ret;
}