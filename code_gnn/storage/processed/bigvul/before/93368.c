int dev_change_net_namespace(struct net_device *dev, struct net *net, const char *pat)
{
	int err;

	ASSERT_RTNL();

	 
	err = -EINVAL;
	if (dev->features & NETIF_F_NETNS_LOCAL)
		goto out;

	 
	if (dev->reg_state != NETREG_REGISTERED)
		goto out;

	 
	err = 0;
	if (net_eq(dev_net(dev), net))
		goto out;

	 
	err = -EEXIST;
	if (__dev_get_by_name(net, dev->name)) {
		 
		if (!pat)
			goto out;
		if (dev_get_valid_name(net, dev, pat) < 0)
			goto out;
	}

	 

	 
	dev_close(dev);

	 
	err = -ENODEV;
	unlist_netdevice(dev);

	synchronize_net();

	 
	dev_shutdown(dev);

	 
	call_netdevice_notifiers(NETDEV_UNREGISTER, dev);
	rcu_barrier();
	call_netdevice_notifiers(NETDEV_UNREGISTER_FINAL, dev);
	rtmsg_ifinfo(RTM_DELLINK, dev, ~0U, GFP_KERNEL);

	 
	dev_uc_flush(dev);
	dev_mc_flush(dev);

	 
	kobject_uevent(&dev->dev.kobj, KOBJ_REMOVE);
	netdev_adjacent_del_links(dev);

	 
	dev_net_set(dev, net);

	 
	if (__dev_get_by_index(net, dev->ifindex))
		dev->ifindex = dev_new_index(net);

	 
	kobject_uevent(&dev->dev.kobj, KOBJ_ADD);
	netdev_adjacent_add_links(dev);

	 
	err = device_rename(&dev->dev, dev->name);
	WARN_ON(err);

	 
	list_netdevice(dev);

	 
	call_netdevice_notifiers(NETDEV_REGISTER, dev);

	 
	rtmsg_ifinfo(RTM_NEWLINK, dev, ~0U, GFP_KERNEL);

	synchronize_net();
	err = 0;
out:
	return err;
}
