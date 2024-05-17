ipv6_add_addr(struct inet6_dev *idev, const struct in6_addr *addr,
	      const struct in6_addr *peer_addr, int pfxlen,
	      int scope, u32 flags, u32 valid_lft, u32 prefered_lft)
{
	struct inet6_ifaddr *ifa = NULL;
	struct rt6_info *rt;
	unsigned int hash;
	int err = 0;
	int addr_type = ipv6_addr_type(addr);

	if (addr_type == IPV6_ADDR_ANY ||
	    addr_type & IPV6_ADDR_MULTICAST ||
	    (!(idev->dev->flags & IFF_LOOPBACK) &&
	     addr_type & IPV6_ADDR_LOOPBACK))
		return ERR_PTR(-EADDRNOTAVAIL);

	rcu_read_lock_bh();
	if (idev->dead) {
		err = -ENODEV;			 
		goto out2;
	}

	if (idev->cnf.disable_ipv6) {
		err = -EACCES;
		goto out2;
	}

	spin_lock(&addrconf_hash_lock);

	 
	if (ipv6_chk_same_addr(dev_net(idev->dev), addr, idev->dev)) {
		ADBG("ipv6_add_addr: already assigned\n");
		err = -EEXIST;
		goto out;
	}

	ifa = kzalloc(sizeof(struct inet6_ifaddr), GFP_ATOMIC);

	if (ifa == NULL) {
		ADBG("ipv6_add_addr: malloc failed\n");
		err = -ENOBUFS;
		goto out;
	}

	rt = addrconf_dst_alloc(idev, addr, false);
	if (IS_ERR(rt)) {
		err = PTR_ERR(rt);
		goto out;
	}

	neigh_parms_data_state_setall(idev->nd_parms);

	ifa->addr = *addr;
	if (peer_addr)
		ifa->peer_addr = *peer_addr;

	spin_lock_init(&ifa->lock);
	spin_lock_init(&ifa->state_lock);
	INIT_DELAYED_WORK(&ifa->dad_work, addrconf_dad_work);
	INIT_HLIST_NODE(&ifa->addr_lst);
	ifa->scope = scope;
	ifa->prefix_len = pfxlen;
	ifa->flags = flags | IFA_F_TENTATIVE;
	ifa->valid_lft = valid_lft;
	ifa->prefered_lft = prefered_lft;
	ifa->cstamp = ifa->tstamp = jiffies;
	ifa->tokenized = false;

	ifa->rt = rt;

	ifa->idev = idev;
	in6_dev_hold(idev);
	 
	in6_ifa_hold(ifa);

	 
	hash = inet6_addr_hash(addr);

	hlist_add_head_rcu(&ifa->addr_lst, &inet6_addr_lst[hash]);
	spin_unlock(&addrconf_hash_lock);

	write_lock(&idev->lock);
	 
	ipv6_link_dev_addr(idev, ifa);

	if (ifa->flags&IFA_F_TEMPORARY) {
		list_add(&ifa->tmp_list, &idev->tempaddr_list);
		in6_ifa_hold(ifa);
	}

	in6_ifa_hold(ifa);
	write_unlock(&idev->lock);
out2:
	rcu_read_unlock_bh();

	if (likely(err == 0))
		inet6addr_notifier_call_chain(NETDEV_UP, ifa);
	else {
		kfree(ifa);
		ifa = ERR_PTR(err);
	}

	return ifa;
out:
	spin_unlock(&addrconf_hash_lock);
	goto out2;
}