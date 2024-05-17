static int ip6mr_mfc_add(struct net *net, struct mr6_table *mrt,
			 struct mf6cctl *mfc, int mrtsock, int parent)
{
	bool found = false;
	int line;
	struct mfc6_cache *uc, *c;
	unsigned char ttls[MAXMIFS];
	int i;

	if (mfc->mf6cc_parent >= MAXMIFS)
		return -ENFILE;

	memset(ttls, 255, MAXMIFS);
	for (i = 0; i < MAXMIFS; i++) {
		if (IF_ISSET(i, &mfc->mf6cc_ifset))
			ttls[i] = 1;

	}

	line = MFC6_HASH(&mfc->mf6cc_mcastgrp.sin6_addr, &mfc->mf6cc_origin.sin6_addr);

	list_for_each_entry(c, &mrt->mfc6_cache_array[line], list) {
		if (ipv6_addr_equal(&c->mf6c_origin, &mfc->mf6cc_origin.sin6_addr) &&
		    ipv6_addr_equal(&c->mf6c_mcastgrp,
				    &mfc->mf6cc_mcastgrp.sin6_addr) &&
		    (parent == -1 || parent == mfc->mf6cc_parent)) {
			found = true;
			break;
		}
	}

	if (found) {
		write_lock_bh(&mrt_lock);
		c->mf6c_parent = mfc->mf6cc_parent;
		ip6mr_update_thresholds(mrt, c, ttls);
		if (!mrtsock)
			c->mfc_flags |= MFC_STATIC;
		write_unlock_bh(&mrt_lock);
		mr6_netlink_event(mrt, c, RTM_NEWROUTE);
		return 0;
	}

	if (!ipv6_addr_any(&mfc->mf6cc_mcastgrp.sin6_addr) &&
	    !ipv6_addr_is_multicast(&mfc->mf6cc_mcastgrp.sin6_addr))
		return -EINVAL;

	c = ip6mr_cache_alloc();
	if (!c)
		return -ENOMEM;

	c->mf6c_origin = mfc->mf6cc_origin.sin6_addr;
	c->mf6c_mcastgrp = mfc->mf6cc_mcastgrp.sin6_addr;
	c->mf6c_parent = mfc->mf6cc_parent;
	ip6mr_update_thresholds(mrt, c, ttls);
	if (!mrtsock)
		c->mfc_flags |= MFC_STATIC;

	write_lock_bh(&mrt_lock);
	list_add(&c->list, &mrt->mfc6_cache_array[line]);
	write_unlock_bh(&mrt_lock);

	 
	found = false;
	spin_lock_bh(&mfc_unres_lock);
	list_for_each_entry(uc, &mrt->mfc6_unres_queue, list) {
		if (ipv6_addr_equal(&uc->mf6c_origin, &c->mf6c_origin) &&
		    ipv6_addr_equal(&uc->mf6c_mcastgrp, &c->mf6c_mcastgrp)) {
			list_del(&uc->list);
			atomic_dec(&mrt->cache_resolve_queue_len);
			found = true;
			break;
		}
	}
	if (list_empty(&mrt->mfc6_unres_queue))
		del_timer(&mrt->ipmr_expire_timer);
	spin_unlock_bh(&mfc_unres_lock);

	if (found) {
		ip6mr_cache_resolve(net, mrt, uc, c);
		ip6mr_cache_free(uc);
	}
	mr6_netlink_event(mrt, c, RTM_NEWROUTE);
	return 0;
}