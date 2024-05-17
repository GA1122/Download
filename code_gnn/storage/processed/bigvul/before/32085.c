static void __net_exit default_device_exit(struct net *net)
{
	struct net_device *dev, *aux;
	 
	rtnl_lock();
	for_each_netdev_safe(net, dev, aux) {
		int err;
		char fb_name[IFNAMSIZ];

		 
		if (dev->features & NETIF_F_NETNS_LOCAL)
			continue;

		 
		if (dev->rtnl_link_ops)
			continue;

		 
		snprintf(fb_name, IFNAMSIZ, "dev%d", dev->ifindex);
		err = dev_change_net_namespace(dev, &init_net, fb_name);
		if (err) {
			printk(KERN_EMERG "%s: failed to move %s to init_net: %d\n",
				__func__, dev->name, err);
			BUG();
		}
	}
	rtnl_unlock();
}
