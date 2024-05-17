static int macvlan_set_mac_address(struct net_device *dev, void *p)
{
	struct macvlan_dev *vlan = netdev_priv(dev);
	struct net_device *lowerdev = vlan->lowerdev;
	struct sockaddr *addr = p;
	int err;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

	if (!(dev->flags & IFF_UP)) {
		 
		memcpy(dev->dev_addr, addr->sa_data, ETH_ALEN);
	} else {
		 
		if (macvlan_addr_busy(vlan->port, addr->sa_data))
			return -EBUSY;

		err = dev_uc_add(lowerdev, addr->sa_data);
		if (err)
			return err;

		dev_uc_del(lowerdev, dev->dev_addr);

		macvlan_hash_change_addr(vlan, addr->sa_data);
	}
	return 0;
}