static int macvlan_device_event(struct notifier_block *unused,
				unsigned long event, void *ptr)
{
	struct net_device *dev = ptr;
	struct macvlan_dev *vlan, *next;
	struct macvlan_port *port;
	LIST_HEAD(list_kill);

	if (!macvlan_port_exists(dev))
		return NOTIFY_DONE;

	port = macvlan_port_get(dev);

	switch (event) {
	case NETDEV_CHANGE:
		list_for_each_entry(vlan, &port->vlans, list)
			netif_stacked_transfer_operstate(vlan->lowerdev,
							 vlan->dev);
		break;
	case NETDEV_FEAT_CHANGE:
		list_for_each_entry(vlan, &port->vlans, list) {
			vlan->dev->features = dev->features & MACVLAN_FEATURES;
			vlan->dev->gso_max_size = dev->gso_max_size;
			netdev_features_change(vlan->dev);
		}
		break;
	case NETDEV_UNREGISTER:
		 
		if (dev->reg_state != NETREG_UNREGISTERING)
			break;

		list_for_each_entry_safe(vlan, next, &port->vlans, list)
			vlan->dev->rtnl_link_ops->dellink(vlan->dev, &list_kill);
		unregister_netdevice_many(&list_kill);
		list_del(&list_kill);
		break;
	case NETDEV_PRE_TYPE_CHANGE:
		 
		return NOTIFY_BAD;
	}
	return NOTIFY_DONE;
}