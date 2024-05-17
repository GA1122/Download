static void __net_exit default_device_exit_batch(struct list_head *net_list)
{
	 
	struct net_device *dev;
	struct net *net;
	LIST_HEAD(dev_kill_list);

	 
	rtnl_lock_unregistering(net_list);
	list_for_each_entry(net, net_list, exit_list) {
		for_each_netdev_reverse(net, dev) {
			if (dev->rtnl_link_ops && dev->rtnl_link_ops->dellink)
				dev->rtnl_link_ops->dellink(dev, &dev_kill_list);
			else
				unregister_netdevice_queue(dev, &dev_kill_list);
		}
	}
	unregister_netdevice_many(&dev_kill_list);
	rtnl_unlock();
}
