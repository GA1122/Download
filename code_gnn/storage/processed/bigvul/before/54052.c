struct net_device *__ip_dev_find(struct net *net, __be32 addr, bool devref)
{
	u32 hash = inet_addr_hash(net, addr);
	struct net_device *result = NULL;
	struct in_ifaddr *ifa;

	rcu_read_lock();
	hlist_for_each_entry_rcu(ifa, &inet_addr_lst[hash], hash) {
		if (ifa->ifa_local == addr) {
			struct net_device *dev = ifa->ifa_dev->dev;

			if (!net_eq(dev_net(dev), net))
				continue;
			result = dev;
			break;
		}
	}
	if (!result) {
		struct flowi4 fl4 = { .daddr = addr };
		struct fib_result res = { 0 };
		struct fib_table *local;

		 
		local = fib_get_table(net, RT_TABLE_LOCAL);
		if (local &&
		    !fib_table_lookup(local, &fl4, &res, FIB_LOOKUP_NOREF) &&
		    res.type == RTN_LOCAL)
			result = FIB_RES_DEV(res);
	}
	if (result && devref)
		dev_hold(result);
	rcu_read_unlock();
	return result;
}