static int addrconf_notify(struct notifier_block *this, unsigned long event,
			   void *ptr)
{
	struct net_device *dev = netdev_notifier_info_to_dev(ptr);
	struct inet6_dev *idev = __in6_dev_get(dev);
	int run_pending = 0;
	int err;

	switch (event) {
	case NETDEV_REGISTER:
		if (!idev && dev->mtu >= IPV6_MIN_MTU) {
			idev = ipv6_add_dev(dev);
			if (IS_ERR(idev))
				return notifier_from_errno(PTR_ERR(idev));
		}
		break;

	case NETDEV_UP:
	case NETDEV_CHANGE:
		if (dev->flags & IFF_SLAVE)
			break;

		if (idev && idev->cnf.disable_ipv6)
			break;

		if (event == NETDEV_UP) {
			if (!addrconf_qdisc_ok(dev)) {
				 
				pr_info("ADDRCONF(NETDEV_UP): %s: link is not ready\n",
					dev->name);
				break;
			}

			if (!idev && dev->mtu >= IPV6_MIN_MTU)
				idev = ipv6_add_dev(dev);

			if (!IS_ERR_OR_NULL(idev)) {
				idev->if_flags |= IF_READY;
				run_pending = 1;
			}
		} else {
			if (!addrconf_qdisc_ok(dev)) {
				 
				break;
			}

			if (idev) {
				if (idev->if_flags & IF_READY)
					 
					break;
				idev->if_flags |= IF_READY;
			}

			pr_info("ADDRCONF(NETDEV_CHANGE): %s: link becomes ready\n",
				dev->name);

			run_pending = 1;
		}

		switch (dev->type) {
#if IS_ENABLED(CONFIG_IPV6_SIT)
		case ARPHRD_SIT:
			addrconf_sit_config(dev);
			break;
#endif
#if IS_ENABLED(CONFIG_NET_IPGRE)
		case ARPHRD_IPGRE:
			addrconf_gre_config(dev);
			break;
#endif
		case ARPHRD_LOOPBACK:
			init_loopback(dev);
			break;

		default:
			addrconf_dev_config(dev);
			break;
		}

		if (!IS_ERR_OR_NULL(idev)) {
			if (run_pending)
				addrconf_dad_run(idev);

			 
			if (idev->cnf.mtu6 != dev->mtu &&
			    dev->mtu >= IPV6_MIN_MTU) {
				rt6_mtu_change(dev, dev->mtu);
				idev->cnf.mtu6 = dev->mtu;
			}
			idev->tstamp = jiffies;
			inet6_ifinfo_notify(RTM_NEWLINK, idev);

			 
			if (dev->mtu < IPV6_MIN_MTU)
				addrconf_ifdown(dev, 1);
		}
		break;

	case NETDEV_CHANGEMTU:
		if (idev && dev->mtu >= IPV6_MIN_MTU) {
			rt6_mtu_change(dev, dev->mtu);
			idev->cnf.mtu6 = dev->mtu;
			break;
		}

		if (!idev && dev->mtu >= IPV6_MIN_MTU) {
			idev = ipv6_add_dev(dev);
			if (!IS_ERR(idev))
				break;
		}

		 

	case NETDEV_DOWN:
	case NETDEV_UNREGISTER:
		 
		addrconf_ifdown(dev, event != NETDEV_DOWN);
		break;

	case NETDEV_CHANGENAME:
		if (idev) {
			snmp6_unregister_dev(idev);
			addrconf_sysctl_unregister(idev);
			err = addrconf_sysctl_register(idev);
			if (err)
				return notifier_from_errno(err);
			err = snmp6_register_dev(idev);
			if (err) {
				addrconf_sysctl_unregister(idev);
				return notifier_from_errno(err);
			}
		}
		break;

	case NETDEV_PRE_TYPE_CHANGE:
	case NETDEV_POST_TYPE_CHANGE:
		addrconf_type_change(dev, event);
		break;
	}

	return NOTIFY_OK;
}