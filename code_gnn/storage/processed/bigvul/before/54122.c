static int fib_inetaddr_event(struct notifier_block *this, unsigned long event, void *ptr)
{
	struct in_ifaddr *ifa = (struct in_ifaddr *)ptr;
	struct net_device *dev = ifa->ifa_dev->dev;
	struct net *net = dev_net(dev);

	switch (event) {
	case NETDEV_UP:
		fib_add_ifaddr(ifa);
#ifdef CONFIG_IP_ROUTE_MULTIPATH
		fib_sync_up(dev, RTNH_F_DEAD);
#endif
		atomic_inc(&net->ipv4.dev_addr_genid);
		rt_cache_flush(dev_net(dev));
		break;
	case NETDEV_DOWN:
		fib_del_ifaddr(ifa, NULL);
		atomic_inc(&net->ipv4.dev_addr_genid);
		if (!ifa->ifa_dev->ifa_list) {
			 
			fib_disable_ip(dev, event, true);
		} else {
			rt_cache_flush(dev_net(dev));
		}
		break;
	}
	return NOTIFY_DONE;
}