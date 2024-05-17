check_cleanup_prefix_route(struct inet6_ifaddr *ifp, unsigned long *expires)
{
	struct inet6_ifaddr *ifa;
	struct inet6_dev *idev = ifp->idev;
	unsigned long lifetime;
	enum cleanup_prefix_rt_t action = CLEANUP_PREFIX_RT_DEL;

	*expires = jiffies;

	list_for_each_entry(ifa, &idev->addr_list, if_list) {
		if (ifa == ifp)
			continue;
		if (!ipv6_prefix_equal(&ifa->addr, &ifp->addr,
				       ifp->prefix_len))
			continue;
		if (ifa->flags & (IFA_F_PERMANENT | IFA_F_NOPREFIXROUTE))
			return CLEANUP_PREFIX_RT_NOP;

		action = CLEANUP_PREFIX_RT_EXPIRE;

		spin_lock(&ifa->lock);

		lifetime = addrconf_timeout_fixup(ifa->valid_lft, HZ);
		 
		if (time_before(*expires, ifa->tstamp + lifetime * HZ))
			*expires = ifa->tstamp + lifetime * HZ;
		spin_unlock(&ifa->lock);
	}

	return action;
}