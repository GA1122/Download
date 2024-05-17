ip6mr_cache_unresolved(struct mr6_table *mrt, mifi_t mifi, struct sk_buff *skb)
{
	bool found = false;
	int err;
	struct mfc6_cache *c;

	spin_lock_bh(&mfc_unres_lock);
	list_for_each_entry(c, &mrt->mfc6_unres_queue, list) {
		if (ipv6_addr_equal(&c->mf6c_mcastgrp, &ipv6_hdr(skb)->daddr) &&
		    ipv6_addr_equal(&c->mf6c_origin, &ipv6_hdr(skb)->saddr)) {
			found = true;
			break;
		}
	}

	if (!found) {
		 

		if (atomic_read(&mrt->cache_resolve_queue_len) >= 10 ||
		    (c = ip6mr_cache_alloc_unres()) == NULL) {
			spin_unlock_bh(&mfc_unres_lock);

			kfree_skb(skb);
			return -ENOBUFS;
		}

		 
		c->mf6c_parent = -1;
		c->mf6c_origin = ipv6_hdr(skb)->saddr;
		c->mf6c_mcastgrp = ipv6_hdr(skb)->daddr;

		 
		err = ip6mr_cache_report(mrt, skb, mifi, MRT6MSG_NOCACHE);
		if (err < 0) {
			 
			spin_unlock_bh(&mfc_unres_lock);

			ip6mr_cache_free(c);
			kfree_skb(skb);
			return err;
		}

		atomic_inc(&mrt->cache_resolve_queue_len);
		list_add(&c->list, &mrt->mfc6_unres_queue);
		mr6_netlink_event(mrt, c, RTM_NEWROUTE);

		ipmr_do_expire_process(mrt);
	}

	 
	if (c->mfc_un.unres.unresolved.qlen > 3) {
		kfree_skb(skb);
		err = -ENOBUFS;
	} else {
		skb_queue_tail(&c->mfc_un.unres.unresolved, skb);
		err = 0;
	}

	spin_unlock_bh(&mfc_unres_lock);
	return err;
}