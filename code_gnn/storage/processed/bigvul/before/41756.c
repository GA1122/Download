void addrconf_dad_failure(struct inet6_ifaddr *ifp)
{
	struct inet6_dev *idev = ifp->idev;

	if (addrconf_dad_end(ifp)) {
		in6_ifa_put(ifp);
		return;
	}

	net_info_ratelimited("%s: IPv6 duplicate address %pI6c detected!\n",
			     ifp->idev->dev->name, &ifp->addr);

	if (idev->cnf.accept_dad > 1 && !idev->cnf.disable_ipv6) {
		struct in6_addr addr;

		addr.s6_addr32[0] = htonl(0xfe800000);
		addr.s6_addr32[1] = 0;

		if (!ipv6_generate_eui64(addr.s6_addr + 8, idev->dev) &&
		    ipv6_addr_equal(&ifp->addr, &addr)) {
			 
			idev->cnf.disable_ipv6 = 1;

			pr_info("%s: IPv6 being disabled!\n",
				ifp->idev->dev->name);
		}
	}

	spin_lock_bh(&ifp->state_lock);
	 
	ifp->state = INET6_IFADDR_STATE_ERRDAD;
	spin_unlock_bh(&ifp->state_lock);

	addrconf_mod_dad_work(ifp, 0);
}