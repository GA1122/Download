static int inetdev_event(struct notifier_block *this, unsigned long event,
			 void *ptr)
{
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	struct in_device *in_dev = __in_dev_get_rtnl(dev);

	ASSERT_RTNL();

	if (!in_dev) {
		if (event == NETDEV_REGISTER) {
			in_dev = inetdev_init(dev);
			if (IS_ERR(in_dev))
				return notifier_from_errno(PTR_ERR(in_dev));
			if (dev->flags & IFF_LOOPBACK) {
				IN_DEV_CONF_SET(in_dev, NOXFRM, 1);
				IN_DEV_CONF_SET(in_dev, NOPOLICY, 1);
			}
		} else if (event == NETDEV_CHANGEMTU) {
			 
			if (inetdev_valid_mtu(dev->mtu))
				in_dev = inetdev_init(dev);
		}
		goto out;
	}

	switch (event) {
	case NETDEV_REGISTER:
		pr_debug("%s: bug\n", __func__);
		RCU_INIT_POINTER(dev->ip_ptr, NULL);
		break;
	case NETDEV_UP:
		if (!inetdev_valid_mtu(dev->mtu))
			break;
		if (dev->flags & IFF_LOOPBACK) {
			struct in_ifaddr *ifa = inet_alloc_ifa();

			if (ifa) {
				INIT_HLIST_NODE(&ifa->hash);
				ifa->ifa_local =
				  ifa->ifa_address = htonl(INADDR_LOOPBACK);
				ifa->ifa_prefixlen = 8;
				ifa->ifa_mask = inet_make_mask(8);
				in_dev_hold(in_dev);
				ifa->ifa_dev = in_dev;
				ifa->ifa_scope = RT_SCOPE_HOST;
				memcpy(ifa->ifa_label, dev->name, IFNAMSIZ);
				set_ifa_lifetime(ifa, INFINITY_LIFE_TIME,
						 INFINITY_LIFE_TIME);
				ipv4_devconf_setall(in_dev);
				neigh_parms_data_state_setall(in_dev->arp_parms);
				inet_insert_ifa(ifa);
			}
		}
		ip_mc_up(in_dev);
		 
	case NETDEV_CHANGEADDR:
		if (!IN_DEV_ARP_NOTIFY(in_dev))
			break;
		 
	case NETDEV_NOTIFY_PEERS:
		 
		inetdev_send_gratuitous_arp(dev, in_dev);
		break;
	case NETDEV_DOWN:
		ip_mc_down(in_dev);
		break;
	case NETDEV_PRE_TYPE_CHANGE:
		ip_mc_unmap(in_dev);
		break;
	case NETDEV_POST_TYPE_CHANGE:
		ip_mc_remap(in_dev);
		break;
	case NETDEV_CHANGEMTU:
		if (inetdev_valid_mtu(dev->mtu))
			break;
		 
	case NETDEV_UNREGISTER:
		inetdev_destroy(in_dev);
		break;
	case NETDEV_CHANGENAME:
		 
		inetdev_changename(dev, in_dev);

		devinet_sysctl_unregister(in_dev);
		devinet_sysctl_register(in_dev);
		break;
	}
out:
	return NOTIFY_DONE;
}