static void init_loopback(struct net_device *dev)
{
	struct inet6_dev  *idev;
	struct net_device *sp_dev;
	struct inet6_ifaddr *sp_ifa;
	struct rt6_info *sp_rt;

	 

	ASSERT_RTNL();

	idev = ipv6_find_idev(dev);
	if (idev == NULL) {
		pr_debug("%s: add_dev failed\n", __func__);
		return;
	}

	add_addr(idev, &in6addr_loopback, 128, IFA_HOST);

	 
	for_each_netdev(dev_net(dev), sp_dev) {
		if (!strcmp(sp_dev->name, dev->name))
			continue;

		idev = __in6_dev_get(sp_dev);
		if (!idev)
			continue;

		read_lock_bh(&idev->lock);
		list_for_each_entry(sp_ifa, &idev->addr_list, if_list) {

			if (sp_ifa->flags & (IFA_F_DADFAILED | IFA_F_TENTATIVE))
				continue;

			if (sp_ifa->rt) {
				 
				if (sp_ifa->rt->dst.obsolete > 0) {
					ip6_rt_put(sp_ifa->rt);
					sp_ifa->rt = NULL;
				} else {
					continue;
				}
			}

			sp_rt = addrconf_dst_alloc(idev, &sp_ifa->addr, false);

			 
			if (!IS_ERR(sp_rt)) {
				sp_ifa->rt = sp_rt;
				ip6_ins_rt(sp_rt);
			}
		}
		read_unlock_bh(&idev->lock);
	}
}