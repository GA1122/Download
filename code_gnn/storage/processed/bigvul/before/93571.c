static void mroute_clean_tables(struct mr6_table *mrt, bool all)
{
	int i;
	LIST_HEAD(list);
	struct mfc6_cache *c, *next;

	 
	for (i = 0; i < mrt->maxvif; i++) {
		if (!all && (mrt->vif6_table[i].flags & VIFF_STATIC))
			continue;
		mif6_delete(mrt, i, &list);
	}
	unregister_netdevice_many(&list);

	 
	for (i = 0; i < MFC6_LINES; i++) {
		list_for_each_entry_safe(c, next, &mrt->mfc6_cache_array[i], list) {
			if (!all && (c->mfc_flags & MFC_STATIC))
				continue;
			write_lock_bh(&mrt_lock);
			list_del(&c->list);
			write_unlock_bh(&mrt_lock);

			mr6_netlink_event(mrt, c, RTM_DELROUTE);
			ip6mr_cache_free(c);
		}
	}

	if (atomic_read(&mrt->cache_resolve_queue_len) != 0) {
		spin_lock_bh(&mfc_unres_lock);
		list_for_each_entry_safe(c, next, &mrt->mfc6_unres_queue, list) {
			list_del(&c->list);
			mr6_netlink_event(mrt, c, RTM_DELROUTE);
			ip6mr_destroy_unres(mrt, c);
		}
		spin_unlock_bh(&mfc_unres_lock);
	}
}