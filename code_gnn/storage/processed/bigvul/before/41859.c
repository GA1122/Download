static struct inet6_dev *ipv6_add_dev(struct net_device *dev)
{
	struct inet6_dev *ndev;
	int err = -ENOMEM;

	ASSERT_RTNL();

	if (dev->mtu < IPV6_MIN_MTU)
		return ERR_PTR(-EINVAL);

	ndev = kzalloc(sizeof(struct inet6_dev), GFP_KERNEL);
	if (ndev == NULL)
		return ERR_PTR(err);

	rwlock_init(&ndev->lock);
	ndev->dev = dev;
	INIT_LIST_HEAD(&ndev->addr_list);
	setup_timer(&ndev->rs_timer, addrconf_rs_timer,
		    (unsigned long)ndev);
	memcpy(&ndev->cnf, dev_net(dev)->ipv6.devconf_dflt, sizeof(ndev->cnf));
	ndev->cnf.mtu6 = dev->mtu;
	ndev->cnf.sysctl = NULL;
	ndev->nd_parms = neigh_parms_alloc(dev, &nd_tbl);
	if (ndev->nd_parms == NULL) {
		kfree(ndev);
		return ERR_PTR(err);
	}
	if (ndev->cnf.forwarding)
		dev_disable_lro(dev);
	 
	dev_hold(dev);

	if (snmp6_alloc_dev(ndev) < 0) {
		ADBG(KERN_WARNING
			"%s: cannot allocate memory for statistics; dev=%s.\n",
			__func__, dev->name);
		neigh_parms_release(&nd_tbl, ndev->nd_parms);
		dev_put(dev);
		kfree(ndev);
		return ERR_PTR(err);
	}

	if (snmp6_register_dev(ndev) < 0) {
		ADBG(KERN_WARNING
			"%s: cannot create /proc/net/dev_snmp6/%s\n",
			__func__, dev->name);
		goto err_release;
	}

	 
	in6_dev_hold(ndev);

	if (dev->flags & (IFF_NOARP | IFF_LOOPBACK))
		ndev->cnf.accept_dad = -1;

#if IS_ENABLED(CONFIG_IPV6_SIT)
	if (dev->type == ARPHRD_SIT && (dev->priv_flags & IFF_ISATAP)) {
		pr_info("%s: Disabled Multicast RS\n", dev->name);
		ndev->cnf.rtr_solicits = 0;
	}
#endif

	INIT_LIST_HEAD(&ndev->tempaddr_list);
	setup_timer(&ndev->regen_timer, ipv6_regen_rndid, (unsigned long)ndev);
	if ((dev->flags&IFF_LOOPBACK) ||
	    dev->type == ARPHRD_TUNNEL ||
	    dev->type == ARPHRD_TUNNEL6 ||
	    dev->type == ARPHRD_SIT ||
	    dev->type == ARPHRD_NONE) {
		ndev->cnf.use_tempaddr = -1;
	} else {
		in6_dev_hold(ndev);
		ipv6_regen_rndid((unsigned long) ndev);
	}

	ndev->token = in6addr_any;

	if (netif_running(dev) && addrconf_qdisc_ok(dev))
		ndev->if_flags |= IF_READY;

	ipv6_mc_init_dev(ndev);
	ndev->tstamp = jiffies;
	err = addrconf_sysctl_register(ndev);
	if (err) {
		ipv6_mc_destroy_dev(ndev);
		del_timer(&ndev->regen_timer);
		goto err_release;
	}
	 
	rcu_assign_pointer(dev->ip6_ptr, ndev);

	 
	ipv6_dev_mc_inc(dev, &in6addr_interfacelocal_allnodes);

	 
	ipv6_dev_mc_inc(dev, &in6addr_linklocal_allnodes);

	 
	if (ndev->cnf.forwarding && (dev->flags & IFF_MULTICAST))
		ipv6_dev_mc_inc(dev, &in6addr_linklocal_allrouters);

	return ndev;

err_release:
	neigh_parms_release(&nd_tbl, ndev->nd_parms);
	ndev->dead = 1;
	in6_dev_finish_destroy(ndev);
	return ERR_PTR(err);
}