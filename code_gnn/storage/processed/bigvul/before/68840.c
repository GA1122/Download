static void *cache_alloc_refill(struct kmem_cache *cachep, gfp_t flags)
{
	int batchcount;
	struct kmem_cache_node *n;
	struct array_cache *ac, *shared;
	int node;
	void *list = NULL;
	struct page *page;

	check_irq_off();
	node = numa_mem_id();

	ac = cpu_cache_get(cachep);
	batchcount = ac->batchcount;
	if (!ac->touched && batchcount > BATCHREFILL_LIMIT) {
		 
		batchcount = BATCHREFILL_LIMIT;
	}
	n = get_node(cachep, node);

	BUG_ON(ac->avail > 0 || !n);
	shared = READ_ONCE(n->shared);
	if (!n->free_objects && (!shared || !shared->avail))
		goto direct_grow;

	spin_lock(&n->list_lock);
	shared = READ_ONCE(n->shared);

	 
	if (shared && transfer_objects(ac, shared, batchcount)) {
		shared->touched = 1;
		goto alloc_done;
	}

	while (batchcount > 0) {
		 
		page = get_first_slab(n, false);
		if (!page)
			goto must_grow;

		check_spinlock_acquired(cachep);

		batchcount = alloc_block(cachep, ac, page, batchcount);
		fixup_slab_list(cachep, n, page, &list);
	}

must_grow:
	n->free_objects -= ac->avail;
alloc_done:
	spin_unlock(&n->list_lock);
	fixup_objfreelist_debug(cachep, &list);

direct_grow:
	if (unlikely(!ac->avail)) {
		 
		if (sk_memalloc_socks()) {
			void *obj = cache_alloc_pfmemalloc(cachep, n, flags);

			if (obj)
				return obj;
		}

		page = cache_grow_begin(cachep, gfp_exact_node(flags), node);

		 
		ac = cpu_cache_get(cachep);
		if (!ac->avail && page)
			alloc_block(cachep, ac, page, batchcount);
		cache_grow_end(cachep, page);

		if (!ac->avail)
			return NULL;
	}
	ac->touched = 1;

	return ac->entry[--ac->avail];
}
