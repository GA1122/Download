int dev_change_net_namespace(struct net_device *dev, struct net *net, const char *pat)
{
	int err;

	ASSERT_RTNL();

	 
	err = -EINVAL;
	if (dev->features & NETIF_F_NETNS_LOCAL)
		goto out;

#ifdef CONFIG_SYSFS
	 
	err = -EINVAL;
	if (dev->dev.parent)
		goto out;
#endif

	 
	err = -EINVAL;
	if (dev->reg_state != NETREG_REGISTERED)
		goto out;

	 
	err = 0;
	if (net_eq(dev_net(dev), net))
		goto out;

	 
	err = -EEXIST;
	if (__dev_get_by_name(net, dev->name)) {
		 
		if (!pat)
			goto out;
		if (dev_get_valid_name(net, pat, dev->name, 1))
			goto out;
	}

	 

	 
	dev_close(dev);

	 
	err = -ENODEV;
	unlist_netdevice(dev);

	synchronize_net();

	 
	dev_shutdown(dev);

	 
	call_netdevice_notifiers(NETDEV_UNREGISTER, dev);
	call_netdevice_notifiers(NETDEV_UNREGISTER_BATCH, dev);

	 
	dev_unicast_flush(dev);
	dev_addr_discard(dev);

	netdev_unregister_kobject(dev);

	 
	dev_net_set(dev, net);

	 
	if (__dev_get_by_index(net, dev->ifindex)) {
		int iflink = (dev->iflink == dev->ifindex);
		dev->ifindex = dev_new_index(net);
		if (iflink)
			dev->iflink = dev->ifindex;
	}

	 
	err = netdev_register_kobject(dev);
	WARN_ON(err);

	 
	list_netdevice(dev);

	 
	call_netdevice_notifiers(NETDEV_REGISTER, dev);

	 
	rtmsg_ifinfo(RTM_NEWLINK, dev, ~0U);

	synchronize_net();
	err = 0;
out:
	return err;
}
