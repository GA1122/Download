static void *____cache_alloc_node(struct kmem_cache *cachep, gfp_t flags,
				int nodeid)
{
	struct page *page;
	struct kmem_cache_node *n;
	void *obj = NULL;
	void *list = NULL;

	VM_BUG_ON(nodeid < 0 || nodeid >= MAX_NUMNODES);
	n = get_node(cachep, nodeid);
	BUG_ON(!n);

	check_irq_off();
	spin_lock(&n->list_lock);
	page = get_first_slab(n, false);
	if (!page)
		goto must_grow;

	check_spinlock_acquired_node(cachep, nodeid);

	STATS_INC_NODEALLOCS(cachep);
	STATS_INC_ACTIVE(cachep);
	STATS_SET_HIGH(cachep);

	BUG_ON(page->active == cachep->num);

	obj = slab_get_obj(cachep, page);
	n->free_objects--;

	fixup_slab_list(cachep, n, page, &list);

	spin_unlock(&n->list_lock);
	fixup_objfreelist_debug(cachep, &list);
	return obj;

must_grow:
	spin_unlock(&n->list_lock);
	page = cache_grow_begin(cachep, gfp_exact_node(flags), nodeid);
	if (page) {
		 
		obj = slab_get_obj(cachep, page);
	}
	cache_grow_end(cachep, page);

	return obj ? obj : fallback_alloc(cachep, flags);
}