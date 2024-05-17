int prism2_wds_add(local_info_t *local, u8 *remote_addr,
		   int rtnl_locked)
{
	struct net_device *dev;
	struct list_head *ptr;
	struct hostap_interface *iface, *empty, *match;

	empty = match = NULL;
	read_lock_bh(&local->iface_lock);
	list_for_each(ptr, &local->hostap_interfaces) {
		iface = list_entry(ptr, struct hostap_interface, list);
		if (iface->type != HOSTAP_INTERFACE_WDS)
			continue;

		if (prism2_wds_special_addr(iface->u.wds.remote_addr))
			empty = iface;
		else if (memcmp(iface->u.wds.remote_addr, remote_addr,
				ETH_ALEN) == 0) {
			match = iface;
			break;
		}
	}
	if (!match && empty && !prism2_wds_special_addr(remote_addr)) {
		 
		memcpy(empty->u.wds.remote_addr, remote_addr, ETH_ALEN);
		read_unlock_bh(&local->iface_lock);
		printk(KERN_DEBUG "%s: using pre-allocated WDS netdevice %s\n",
		       local->dev->name, empty->dev->name);
		return 0;
	}
	read_unlock_bh(&local->iface_lock);

	if (!prism2_wds_special_addr(remote_addr)) {
		if (match)
			return -EEXIST;
		hostap_add_sta(local->ap, remote_addr);
	}

	if (local->wds_connections >= local->wds_max_connections)
		return -ENOBUFS;

	 
	if (strlen(local->dev->name) >= IFNAMSIZ - 5) {
		printk(KERN_DEBUG "'%s' too long base device name\n",
		       local->dev->name);
		return -EINVAL;
	}

	dev = hostap_add_interface(local, HOSTAP_INTERFACE_WDS, rtnl_locked,
				   local->ddev->name, "wds%d");
	if (dev == NULL)
		return -ENOMEM;

	iface = netdev_priv(dev);
	memcpy(iface->u.wds.remote_addr, remote_addr, ETH_ALEN);

	local->wds_connections++;

	return 0;
}