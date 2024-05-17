static int macvtap_device_event(struct notifier_block *unused,
				unsigned long event, void *ptr)
{
	struct net_device *dev = ptr;
	struct macvlan_dev *vlan;
	struct device *classdev;
	dev_t devt;
	int err;

	if (dev->rtnl_link_ops != &macvtap_link_ops)
		return NOTIFY_DONE;

	vlan = netdev_priv(dev);

	switch (event) {
	case NETDEV_REGISTER:
		 
		err = macvtap_get_minor(vlan);
		if (err)
			return notifier_from_errno(err);

		devt = MKDEV(MAJOR(macvtap_major), vlan->minor);
		classdev = device_create(macvtap_class, &dev->dev, devt,
					 dev, "tap%d", dev->ifindex);
		if (IS_ERR(classdev)) {
			macvtap_free_minor(vlan);
			return notifier_from_errno(PTR_ERR(classdev));
		}
		break;
	case NETDEV_UNREGISTER:
		devt = MKDEV(MAJOR(macvtap_major), vlan->minor);
		device_destroy(macvtap_class, devt);
		macvtap_free_minor(vlan);
		break;
	}

	return NOTIFY_DONE;
}