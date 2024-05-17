static int pppoe_device_event(struct notifier_block *this,
			      unsigned long event, void *ptr)
{
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);

	 
	switch (event) {
	case NETDEV_CHANGEADDR:
	case NETDEV_CHANGEMTU:
		 

	case NETDEV_GOING_DOWN:
	case NETDEV_DOWN:
		 
		pppoe_flush_dev(dev);
		break;

	default:
		break;
	}

	return NOTIFY_DONE;
}
