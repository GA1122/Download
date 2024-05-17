int dev_set_mtu(struct net_device *dev, int new_mtu)
{
	int err, orig_mtu;

	if (new_mtu == dev->mtu)
		return 0;

	 
	if (new_mtu < 0)
		return -EINVAL;

	if (!netif_device_present(dev))
		return -ENODEV;

	err = call_netdevice_notifiers(NETDEV_PRECHANGEMTU, dev);
	err = notifier_to_errno(err);
	if (err)
		return err;

	orig_mtu = dev->mtu;
	err = __dev_set_mtu(dev, new_mtu);

	if (!err) {
		err = call_netdevice_notifiers(NETDEV_CHANGEMTU, dev);
		err = notifier_to_errno(err);
		if (err) {
			 
			__dev_set_mtu(dev, orig_mtu);
			call_netdevice_notifiers(NETDEV_CHANGEMTU, dev);
		}
	}
	return err;
}