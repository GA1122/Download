static int atif_ioctl(int cmd, void __user *arg)
{
	static char aarp_mcast[6] = { 0x09, 0x00, 0x00, 0xFF, 0xFF, 0xFF };
	struct ifreq atreq;
	struct atalk_netrange *nr;
	struct sockaddr_at *sa;
	struct net_device *dev;
	struct atalk_iface *atif;
	int ct;
	int limit;
	struct rtentry rtdef;
	int add_route;

	if (copy_from_user(&atreq, arg, sizeof(atreq)))
		return -EFAULT;

	dev = __dev_get_by_name(&init_net, atreq.ifr_name);
	if (!dev)
		return -ENODEV;

	sa = (struct sockaddr_at *)&atreq.ifr_addr;
	atif = atalk_find_dev(dev);

	switch (cmd) {
	case SIOCSIFADDR:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		if (sa->sat_family != AF_APPLETALK)
			return -EINVAL;
		if (dev->type != ARPHRD_ETHER &&
		    dev->type != ARPHRD_LOOPBACK &&
		    dev->type != ARPHRD_LOCALTLK &&
		    dev->type != ARPHRD_PPP)
			return -EPROTONOSUPPORT;

		nr = (struct atalk_netrange *)&sa->sat_zero[0];
		add_route = 1;

		 
		if ((dev->flags & IFF_POINTOPOINT) &&
		    atalk_find_interface(sa->sat_addr.s_net,
					 sa->sat_addr.s_node)) {
			printk(KERN_DEBUG "AppleTalk: point-to-point "
			       "interface added with "
			       "existing address\n");
			add_route = 0;
		}

		 
		if (dev->type == ARPHRD_ETHER && nr->nr_phase != 2)
			return -EPROTONOSUPPORT;
		if (sa->sat_addr.s_node == ATADDR_BCAST ||
		    sa->sat_addr.s_node == 254)
			return -EINVAL;
		if (atif) {
			 
			if (atif->status & ATIF_PROBE)
				return -EBUSY;

			atif->address.s_net  = sa->sat_addr.s_net;
			atif->address.s_node = sa->sat_addr.s_node;
			atrtr_device_down(dev);	 
		} else {
			atif = atif_add_device(dev, &sa->sat_addr);
			if (!atif)
				return -ENOMEM;
		}
		atif->nets = *nr;

		 

		if (!(dev->flags & IFF_LOOPBACK) &&
		    !(dev->flags & IFF_POINTOPOINT) &&
		    atif_probe_device(atif) < 0) {
			atif_drop_device(dev);
			return -EADDRINUSE;
		}

		 
		sa = (struct sockaddr_at *)&rtdef.rt_gateway;
		sa->sat_family = AF_APPLETALK;
		sa->sat_addr.s_net  = atif->address.s_net;
		sa->sat_addr.s_node = atif->address.s_node;
		sa = (struct sockaddr_at *)&rtdef.rt_dst;
		rtdef.rt_flags = RTF_UP;
		sa->sat_family = AF_APPLETALK;
		sa->sat_addr.s_node = ATADDR_ANYNODE;
		if (dev->flags & IFF_LOOPBACK ||
		    dev->flags & IFF_POINTOPOINT)
			rtdef.rt_flags |= RTF_HOST;

		 
		if (nr->nr_firstnet == htons(0) &&
		    nr->nr_lastnet == htons(0xFFFE)) {
			sa->sat_addr.s_net = atif->address.s_net;
			atrtr_create(&rtdef, dev);
			atrtr_set_default(dev);
		} else {
			limit = ntohs(nr->nr_lastnet);
			if (limit - ntohs(nr->nr_firstnet) > 4096) {
				printk(KERN_WARNING "Too many routes/"
				       "iface.\n");
				return -EINVAL;
			}
			if (add_route)
				for (ct = ntohs(nr->nr_firstnet);
				     ct <= limit; ct++) {
					sa->sat_addr.s_net = htons(ct);
					atrtr_create(&rtdef, dev);
				}
		}
		dev_mc_add_global(dev, aarp_mcast);
		return 0;

	case SIOCGIFADDR:
		if (!atif)
			return -EADDRNOTAVAIL;

		sa->sat_family = AF_APPLETALK;
		sa->sat_addr = atif->address;
		break;

	case SIOCGIFBRDADDR:
		if (!atif)
			return -EADDRNOTAVAIL;

		sa->sat_family = AF_APPLETALK;
		sa->sat_addr.s_net = atif->address.s_net;
		sa->sat_addr.s_node = ATADDR_BCAST;
		break;

	case SIOCATALKDIFADDR:
	case SIOCDIFADDR:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		if (sa->sat_family != AF_APPLETALK)
			return -EINVAL;
		atalk_dev_down(dev);
		break;

	case SIOCSARP:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		if (sa->sat_family != AF_APPLETALK)
			return -EINVAL;
		 
		if (dev->type != ARPHRD_ETHER)
			return -EPROTONOSUPPORT;

		 
		if (!atif)
			return -EADDRNOTAVAIL;

		nr = (struct atalk_netrange *)&(atif->nets);
		 
		if (dev->type == ARPHRD_ETHER && nr->nr_phase != 2)
			return -EPROTONOSUPPORT;

		if (sa->sat_addr.s_node == ATADDR_BCAST ||
		    sa->sat_addr.s_node == 254)
			return -EINVAL;

		 
		if (atif_proxy_probe_device(atif, &(sa->sat_addr)) < 0)
			return -EADDRINUSE;

		 
		break;

	case SIOCDARP:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;
		if (sa->sat_family != AF_APPLETALK)
			return -EINVAL;
		if (!atif)
			return -EADDRNOTAVAIL;

		 
		aarp_proxy_remove(atif->dev, &(sa->sat_addr));
		return 0;
	}

	return copy_to_user(arg, &atreq, sizeof(atreq)) ? -EFAULT : 0;
}