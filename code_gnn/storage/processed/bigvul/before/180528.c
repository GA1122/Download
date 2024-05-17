 static int masq_inet_event(struct notifier_block *this,
  			   unsigned long event,
  			   void *ptr)
  {
	struct net_device *dev = ((struct in_ifaddr *)ptr)->ifa_dev->dev;
// 	struct in_device *idev = ((struct in_ifaddr *)ptr)->ifa_dev;
  	struct netdev_notifier_info info;
  
	netdev_notifier_info_init(&info, dev);
// 	 
// 	if (idev->dead)
// 		return NOTIFY_DONE;
// 
// 	netdev_notifier_info_init(&info, idev->dev);
  	return masq_device_event(this, event, &info);
  }