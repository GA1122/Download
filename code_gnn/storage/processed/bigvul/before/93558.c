static void ipmr_do_expire_process(struct mr6_table *mrt)
{
	unsigned long now = jiffies;
	unsigned long expires = 10 * HZ;
	struct mfc6_cache *c, *next;

	list_for_each_entry_safe(c, next, &mrt->mfc6_unres_queue, list) {
		if (time_after(c->mfc_un.unres.expires, now)) {
			 
			unsigned long interval = c->mfc_un.unres.expires - now;
			if (interval < expires)
				expires = interval;
			continue;
		}

		list_del(&c->list);
		mr6_netlink_event(mrt, c, RTM_DELROUTE);
		ip6mr_destroy_unres(mrt, c);
	}

	if (!list_empty(&mrt->mfc6_unres_queue))
		mod_timer(&mrt->ipmr_expire_timer, jiffies + expires);
}