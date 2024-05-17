__be32 inet_select_addr(const struct net_device *dev, __be32 dst, int scope)
{
	__be32 addr = 0;
	struct in_device *in_dev;
	struct net *net = dev_net(dev);
	int master_idx;

	rcu_read_lock();
	in_dev = __in_dev_get_rcu(dev);
	if (!in_dev)
		goto no_in_dev;

	for_primary_ifa(in_dev) {
		if (ifa->ifa_scope > scope)
			continue;
		if (!dst || inet_ifa_match(dst, ifa)) {
			addr = ifa->ifa_local;
			break;
		}
		if (!addr)
			addr = ifa->ifa_local;
	} endfor_ifa(in_dev);

	if (addr)
		goto out_unlock;
no_in_dev:
	master_idx = l3mdev_master_ifindex_rcu(dev);

	 
	if (master_idx &&
	    (dev = dev_get_by_index_rcu(net, master_idx)) &&
	    (in_dev = __in_dev_get_rcu(dev))) {
		for_primary_ifa(in_dev) {
			if (ifa->ifa_scope != RT_SCOPE_LINK &&
			    ifa->ifa_scope <= scope) {
				addr = ifa->ifa_local;
				goto out_unlock;
			}
		} endfor_ifa(in_dev);
	}

	 
	for_each_netdev_rcu(net, dev) {
		if (l3mdev_master_ifindex_rcu(dev) != master_idx)
			continue;

		in_dev = __in_dev_get_rcu(dev);
		if (!in_dev)
			continue;

		for_primary_ifa(in_dev) {
			if (ifa->ifa_scope != RT_SCOPE_LINK &&
			    ifa->ifa_scope <= scope) {
				addr = ifa->ifa_local;
				goto out_unlock;
			}
		} endfor_ifa(in_dev);
	}
out_unlock:
	rcu_read_unlock();
	return addr;
}