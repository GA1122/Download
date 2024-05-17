static int addrconf_ifdown(struct net_device *dev, int how)
{
	struct net *net = dev_net(dev);
	struct inet6_dev *idev;
	struct inet6_ifaddr *ifa;
	int state, i;

	ASSERT_RTNL();

	rt6_ifdown(net, dev);
	neigh_ifdown(&nd_tbl, dev);

	idev = __in6_dev_get(dev);
	if (idev == NULL)
		return -ENODEV;

	 
	if (how) {
		idev->dead = 1;

		 
		RCU_INIT_POINTER(dev->ip6_ptr, NULL);

		 
		snmp6_unregister_dev(idev);

	}

	 
	for (i = 0; i < IN6_ADDR_HSIZE; i++) {
		struct hlist_head *h = &inet6_addr_lst[i];

		spin_lock_bh(&addrconf_hash_lock);
restart:
		hlist_for_each_entry_rcu(ifa, h, addr_lst) {
			if (ifa->idev == idev) {
				hlist_del_init_rcu(&ifa->addr_lst);
				addrconf_del_dad_work(ifa);
				goto restart;
			}
		}
		spin_unlock_bh(&addrconf_hash_lock);
	}

	write_lock_bh(&idev->lock);

	addrconf_del_rs_timer(idev);

	 
	if (!how)
		idev->if_flags &= ~(IF_RS_SENT|IF_RA_RCVD|IF_READY);

	if (how && del_timer(&idev->regen_timer))
		in6_dev_put(idev);

	 
	while (!list_empty(&idev->tempaddr_list)) {
		ifa = list_first_entry(&idev->tempaddr_list,
				       struct inet6_ifaddr, tmp_list);
		list_del(&ifa->tmp_list);
		write_unlock_bh(&idev->lock);
		spin_lock_bh(&ifa->lock);

		if (ifa->ifpub) {
			in6_ifa_put(ifa->ifpub);
			ifa->ifpub = NULL;
		}
		spin_unlock_bh(&ifa->lock);
		in6_ifa_put(ifa);
		write_lock_bh(&idev->lock);
	}

	while (!list_empty(&idev->addr_list)) {
		ifa = list_first_entry(&idev->addr_list,
				       struct inet6_ifaddr, if_list);
		addrconf_del_dad_work(ifa);

		list_del(&ifa->if_list);

		write_unlock_bh(&idev->lock);

		spin_lock_bh(&ifa->state_lock);
		state = ifa->state;
		ifa->state = INET6_IFADDR_STATE_DEAD;
		spin_unlock_bh(&ifa->state_lock);

		if (state != INET6_IFADDR_STATE_DEAD) {
			__ipv6_ifa_notify(RTM_DELADDR, ifa);
			inet6addr_notifier_call_chain(NETDEV_DOWN, ifa);
		}
		in6_ifa_put(ifa);

		write_lock_bh(&idev->lock);
	}

	write_unlock_bh(&idev->lock);

	 
	if (how) {
		ipv6_ac_destroy_dev(idev);
		ipv6_mc_destroy_dev(idev);
	} else {
		ipv6_mc_down(idev);
	}

	idev->tstamp = jiffies;

	 
	if (how) {
		addrconf_sysctl_unregister(idev);
		neigh_parms_release(&nd_tbl, idev->nd_parms);
		neigh_ifdown(&nd_tbl, dev);
		in6_dev_put(idev);
	}
	return 0;
}