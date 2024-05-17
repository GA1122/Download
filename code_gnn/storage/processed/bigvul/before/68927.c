static int setup_kmem_cache_node(struct kmem_cache *cachep,
				int node, gfp_t gfp, bool force_change)
{
	int ret = -ENOMEM;
	struct kmem_cache_node *n;
	struct array_cache *old_shared = NULL;
	struct array_cache *new_shared = NULL;
	struct alien_cache **new_alien = NULL;
	LIST_HEAD(list);

	if (use_alien_caches) {
		new_alien = alloc_alien_cache(node, cachep->limit, gfp);
		if (!new_alien)
			goto fail;
	}

	if (cachep->shared) {
		new_shared = alloc_arraycache(node,
			cachep->shared * cachep->batchcount, 0xbaadf00d, gfp);
		if (!new_shared)
			goto fail;
	}

	ret = init_cache_node(cachep, node, gfp);
	if (ret)
		goto fail;

	n = get_node(cachep, node);
	spin_lock_irq(&n->list_lock);
	if (n->shared && force_change) {
		free_block(cachep, n->shared->entry,
				n->shared->avail, node, &list);
		n->shared->avail = 0;
	}

	if (!n->shared || force_change) {
		old_shared = n->shared;
		n->shared = new_shared;
		new_shared = NULL;
	}

	if (!n->alien) {
		n->alien = new_alien;
		new_alien = NULL;
	}

	spin_unlock_irq(&n->list_lock);
	slabs_destroy(cachep, &list);

	 
	if (old_shared && force_change)
		synchronize_sched();

fail:
	kfree(old_shared);
	kfree(new_shared);
	free_alien_cache(new_alien);

	return ret;
}