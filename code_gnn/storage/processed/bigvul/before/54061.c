int devinet_ioctl(struct net *net, unsigned int cmd, void __user *arg)
{
	struct ifreq ifr;
	struct sockaddr_in sin_orig;
	struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;
	struct in_device *in_dev;
	struct in_ifaddr **ifap = NULL;
	struct in_ifaddr *ifa = NULL;
	struct net_device *dev;
	char *colon;
	int ret = -EFAULT;
	int tryaddrmatch = 0;

	 

	if (copy_from_user(&ifr, arg, sizeof(struct ifreq)))
		goto out;
	ifr.ifr_name[IFNAMSIZ - 1] = 0;

	 
	memcpy(&sin_orig, sin, sizeof(*sin));

	colon = strchr(ifr.ifr_name, ':');
	if (colon)
		*colon = 0;

	dev_load(net, ifr.ifr_name);

	switch (cmd) {
	case SIOCGIFADDR:	 
	case SIOCGIFBRDADDR:	 
	case SIOCGIFDSTADDR:	 
	case SIOCGIFNETMASK:	 
		 
		tryaddrmatch = (sin_orig.sin_family == AF_INET);
		memset(sin, 0, sizeof(*sin));
		sin->sin_family = AF_INET;
		break;

	case SIOCSIFFLAGS:
		ret = -EPERM;
		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
			goto out;
		break;
	case SIOCSIFADDR:	 
	case SIOCSIFBRDADDR:	 
	case SIOCSIFDSTADDR:	 
	case SIOCSIFNETMASK: 	 
		ret = -EPERM;
		if (!ns_capable(net->user_ns, CAP_NET_ADMIN))
			goto out;
		ret = -EINVAL;
		if (sin->sin_family != AF_INET)
			goto out;
		break;
	default:
		ret = -EINVAL;
		goto out;
	}

	rtnl_lock();

	ret = -ENODEV;
	dev = __dev_get_by_name(net, ifr.ifr_name);
	if (!dev)
		goto done;

	if (colon)
		*colon = ':';

	in_dev = __in_dev_get_rtnl(dev);
	if (in_dev) {
		if (tryaddrmatch) {
			 
			 
			 
			for (ifap = &in_dev->ifa_list; (ifa = *ifap) != NULL;
			     ifap = &ifa->ifa_next) {
				if (!strcmp(ifr.ifr_name, ifa->ifa_label) &&
				    sin_orig.sin_addr.s_addr ==
							ifa->ifa_local) {
					break;  
				}
			}
		}
		 
		if (!ifa) {
			for (ifap = &in_dev->ifa_list; (ifa = *ifap) != NULL;
			     ifap = &ifa->ifa_next)
				if (!strcmp(ifr.ifr_name, ifa->ifa_label))
					break;
		}
	}

	ret = -EADDRNOTAVAIL;
	if (!ifa && cmd != SIOCSIFADDR && cmd != SIOCSIFFLAGS)
		goto done;

	switch (cmd) {
	case SIOCGIFADDR:	 
		sin->sin_addr.s_addr = ifa->ifa_local;
		goto rarok;

	case SIOCGIFBRDADDR:	 
		sin->sin_addr.s_addr = ifa->ifa_broadcast;
		goto rarok;

	case SIOCGIFDSTADDR:	 
		sin->sin_addr.s_addr = ifa->ifa_address;
		goto rarok;

	case SIOCGIFNETMASK:	 
		sin->sin_addr.s_addr = ifa->ifa_mask;
		goto rarok;

	case SIOCSIFFLAGS:
		if (colon) {
			ret = -EADDRNOTAVAIL;
			if (!ifa)
				break;
			ret = 0;
			if (!(ifr.ifr_flags & IFF_UP))
				inet_del_ifa(in_dev, ifap, 1);
			break;
		}
		ret = dev_change_flags(dev, ifr.ifr_flags);
		break;

	case SIOCSIFADDR:	 
		ret = -EINVAL;
		if (inet_abc_len(sin->sin_addr.s_addr) < 0)
			break;

		if (!ifa) {
			ret = -ENOBUFS;
			ifa = inet_alloc_ifa();
			if (!ifa)
				break;
			INIT_HLIST_NODE(&ifa->hash);
			if (colon)
				memcpy(ifa->ifa_label, ifr.ifr_name, IFNAMSIZ);
			else
				memcpy(ifa->ifa_label, dev->name, IFNAMSIZ);
		} else {
			ret = 0;
			if (ifa->ifa_local == sin->sin_addr.s_addr)
				break;
			inet_del_ifa(in_dev, ifap, 0);
			ifa->ifa_broadcast = 0;
			ifa->ifa_scope = 0;
		}

		ifa->ifa_address = ifa->ifa_local = sin->sin_addr.s_addr;

		if (!(dev->flags & IFF_POINTOPOINT)) {
			ifa->ifa_prefixlen = inet_abc_len(ifa->ifa_address);
			ifa->ifa_mask = inet_make_mask(ifa->ifa_prefixlen);
			if ((dev->flags & IFF_BROADCAST) &&
			    ifa->ifa_prefixlen < 31)
				ifa->ifa_broadcast = ifa->ifa_address |
						     ~ifa->ifa_mask;
		} else {
			ifa->ifa_prefixlen = 32;
			ifa->ifa_mask = inet_make_mask(32);
		}
		set_ifa_lifetime(ifa, INFINITY_LIFE_TIME, INFINITY_LIFE_TIME);
		ret = inet_set_ifa(dev, ifa);
		break;

	case SIOCSIFBRDADDR:	 
		ret = 0;
		if (ifa->ifa_broadcast != sin->sin_addr.s_addr) {
			inet_del_ifa(in_dev, ifap, 0);
			ifa->ifa_broadcast = sin->sin_addr.s_addr;
			inet_insert_ifa(ifa);
		}
		break;

	case SIOCSIFDSTADDR:	 
		ret = 0;
		if (ifa->ifa_address == sin->sin_addr.s_addr)
			break;
		ret = -EINVAL;
		if (inet_abc_len(sin->sin_addr.s_addr) < 0)
			break;
		ret = 0;
		inet_del_ifa(in_dev, ifap, 0);
		ifa->ifa_address = sin->sin_addr.s_addr;
		inet_insert_ifa(ifa);
		break;

	case SIOCSIFNETMASK: 	 

		 
		ret = -EINVAL;
		if (bad_mask(sin->sin_addr.s_addr, 0))
			break;
		ret = 0;
		if (ifa->ifa_mask != sin->sin_addr.s_addr) {
			__be32 old_mask = ifa->ifa_mask;
			inet_del_ifa(in_dev, ifap, 0);
			ifa->ifa_mask = sin->sin_addr.s_addr;
			ifa->ifa_prefixlen = inet_mask_len(ifa->ifa_mask);

			 
			if ((dev->flags & IFF_BROADCAST) &&
			    (ifa->ifa_prefixlen < 31) &&
			    (ifa->ifa_broadcast ==
			     (ifa->ifa_local|~old_mask))) {
				ifa->ifa_broadcast = (ifa->ifa_local |
						      ~sin->sin_addr.s_addr);
			}
			inet_insert_ifa(ifa);
		}
		break;
	}
done:
	rtnl_unlock();
out:
	return ret;
rarok:
	rtnl_unlock();
	ret = copy_to_user(arg, &ifr, sizeof(struct ifreq)) ? -EFAULT : 0;
	goto out;
}