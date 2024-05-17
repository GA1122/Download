static int setup_netdev(struct lxc_netdev *netdev)
{
	char ifname[IFNAMSIZ];
	char *current_ifname = ifname;
	int err;

	 
	if (!netdev->ifindex) {
		if (netdev->flags & IFF_UP) {
			err = lxc_netdev_up("lo");
			if (err) {
				ERROR("failed to set the loopback up : %s",
				      strerror(-err));
				return -1;
			}
		}
		if (netdev->type != LXC_NET_VETH)
			return 0;
		netdev->ifindex = if_nametoindex(netdev->name);
	}

	 
	if (netdev->type == LXC_NET_PHYS) {
		if (!(netdev->ifindex = if_nametoindex(netdev->link))) {
			ERROR("failed to get ifindex for %s",
				netdev->link);
			return -1;
		}
	}

	 
	if (!if_indextoname(netdev->ifindex, current_ifname)) {
		ERROR("no interface corresponding to index '%d'",
		      netdev->ifindex);
		return -1;
	}

	 
	if (!netdev->name)
		netdev->name = netdev->type == LXC_NET_PHYS ?
			netdev->link : "eth%d";

	 
	if (strcmp(ifname, netdev->name) != 0) {
		err = lxc_netdev_rename_by_name(ifname, netdev->name);
		if (err) {
			ERROR("failed to rename %s->%s : %s", ifname, netdev->name,
			      strerror(-err));
			return -1;
		}
	}

	 
	if (!if_indextoname(netdev->ifindex, current_ifname)) {
		ERROR("no interface corresponding to index '%d'",
		      netdev->ifindex);
		return -1;
	}

	 
	if (netdev->hwaddr) {
		if (setup_hw_addr(netdev->hwaddr, current_ifname)) {
			ERROR("failed to setup hw address for '%s'",
			      current_ifname);
			return -1;
		}
	}

	 
	if (setup_ipv4_addr(&netdev->ipv4, netdev->ifindex)) {
		ERROR("failed to setup ip addresses for '%s'",
			      ifname);
		return -1;
	}

	 
	if (setup_ipv6_addr(&netdev->ipv6, netdev->ifindex)) {
		ERROR("failed to setup ipv6 addresses for '%s'",
			      ifname);
		return -1;
	}

	 
	if (netdev->flags & IFF_UP) {
		int err;

		err = lxc_netdev_up(current_ifname);
		if (err) {
			ERROR("failed to set '%s' up : %s", current_ifname,
			      strerror(-err));
			return -1;
		}

		 
		err = lxc_netdev_up("lo");
		if (err) {
			ERROR("failed to set the loopback up : %s",
			      strerror(-err));
			return -1;
		}
	}

	 

	 
	if (netdev->ipv4_gateway) {
		if (!(netdev->flags & IFF_UP)) {
			ERROR("Cannot add ipv4 gateway for %s when not bringing up the interface", ifname);
			return -1;
		}

		if (lxc_list_empty(&netdev->ipv4)) {
			ERROR("Cannot add ipv4 gateway for %s when not assigning an address", ifname);
			return -1;
		}

		err = lxc_ipv4_gateway_add(netdev->ifindex, netdev->ipv4_gateway);
		if (err) {
			err = lxc_ipv4_dest_add(netdev->ifindex, netdev->ipv4_gateway);
			if (err) {
				ERROR("failed to add ipv4 dest for '%s': %s",
					      ifname, strerror(-err));
			}

			err = lxc_ipv4_gateway_add(netdev->ifindex, netdev->ipv4_gateway);
			if (err) {
				ERROR("failed to setup ipv4 gateway for '%s': %s",
					      ifname, strerror(-err));
				if (netdev->ipv4_gateway_auto) {
					char buf[INET_ADDRSTRLEN];
					inet_ntop(AF_INET, netdev->ipv4_gateway, buf, sizeof(buf));
					ERROR("tried to set autodetected ipv4 gateway '%s'", buf);
				}
				return -1;
			}
		}
	}

	 
	if (netdev->ipv6_gateway) {
		if (!(netdev->flags & IFF_UP)) {
			ERROR("Cannot add ipv6 gateway for %s when not bringing up the interface", ifname);
			return -1;
		}

		if (lxc_list_empty(&netdev->ipv6) && !IN6_IS_ADDR_LINKLOCAL(netdev->ipv6_gateway)) {
			ERROR("Cannot add ipv6 gateway for %s when not assigning an address", ifname);
			return -1;
		}

		err = lxc_ipv6_gateway_add(netdev->ifindex, netdev->ipv6_gateway);
		if (err) {
			err = lxc_ipv6_dest_add(netdev->ifindex, netdev->ipv6_gateway);
			if (err) {
				ERROR("failed to add ipv6 dest for '%s': %s",
				      ifname, strerror(-err));
			}

			err = lxc_ipv6_gateway_add(netdev->ifindex, netdev->ipv6_gateway);
			if (err) {
				ERROR("failed to setup ipv6 gateway for '%s': %s",
					      ifname, strerror(-err));
				if (netdev->ipv6_gateway_auto) {
					char buf[INET6_ADDRSTRLEN];
					inet_ntop(AF_INET6, netdev->ipv6_gateway, buf, sizeof(buf));
					ERROR("tried to set autodetected ipv6 gateway '%s'", buf);
				}
				return -1;
			}
		}
	}

	DEBUG("'%s' has been setup", current_ifname);

	return 0;
}
