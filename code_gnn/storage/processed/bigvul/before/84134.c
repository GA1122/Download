static void blkg_destroy(struct blkcg_gq *blkg)
{
	struct blkcg *blkcg = blkg->blkcg;
	struct blkcg_gq *parent = blkg->parent;
	int i;

	lockdep_assert_held(blkg->q->queue_lock);
	lockdep_assert_held(&blkcg->lock);

	 
	WARN_ON_ONCE(list_empty(&blkg->q_node));
	WARN_ON_ONCE(hlist_unhashed(&blkg->blkcg_node));

	for (i = 0; i < BLKCG_MAX_POLS; i++) {
		struct blkcg_policy *pol = blkcg_policy[i];

		if (blkg->pd[i] && pol->pd_offline_fn)
			pol->pd_offline_fn(blkg->pd[i]);
	}

	if (parent) {
		blkg_rwstat_add_aux(&parent->stat_bytes, &blkg->stat_bytes);
		blkg_rwstat_add_aux(&parent->stat_ios, &blkg->stat_ios);
	}

	blkg->online = false;

	radix_tree_delete(&blkcg->blkg_tree, blkg->q->id);
	list_del_init(&blkg->q_node);
	hlist_del_init_rcu(&blkg->blkcg_node);

	 
	if (rcu_access_pointer(blkcg->blkg_hint) == blkg)
		rcu_assign_pointer(blkcg->blkg_hint, NULL);

	 
	blkg_put(blkg);
}