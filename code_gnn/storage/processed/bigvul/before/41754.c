static void addrconf_dad_completed(struct inet6_ifaddr *ifp)
{
	struct net_device *dev = ifp->idev->dev;
	struct in6_addr lladdr;
	bool send_rs, send_mld;

	addrconf_del_dad_work(ifp);

	 

	ipv6_ifa_notify(RTM_NEWADDR, ifp);

	 

	read_lock_bh(&ifp->idev->lock);
	send_mld = ifp->scope == IFA_LINK && ipv6_lonely_lladdr(ifp);
	send_rs = send_mld &&
		  ipv6_accept_ra(ifp->idev) &&
		  ifp->idev->cnf.rtr_solicits > 0 &&
		  (dev->flags&IFF_LOOPBACK) == 0;
	read_unlock_bh(&ifp->idev->lock);

	 
	if (send_mld)
		ipv6_mc_dad_complete(ifp->idev);

	if (send_rs) {
		 
		if (ipv6_get_lladdr(dev, &lladdr, IFA_F_TENTATIVE))
			return;
		ndisc_send_rs(dev, &lladdr, &in6addr_linklocal_allrouters);

		write_lock_bh(&ifp->idev->lock);
		spin_lock(&ifp->lock);
		ifp->idev->rs_probes = 1;
		ifp->idev->if_flags |= IF_RS_SENT;
		addrconf_mod_rs_timer(ifp->idev,
				      ifp->idev->cnf.rtr_solicit_interval);
		spin_unlock(&ifp->lock);
		write_unlock_bh(&ifp->idev->lock);
	}
}