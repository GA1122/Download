int kmem_cache_shrink(struct kmem_cache *s)
{
	int node;
	int i;
	struct kmem_cache_node *n;
	struct page *page;
	struct page *t;
	int objects = oo_objects(s->max);
	struct list_head *slabs_by_inuse =
		kmalloc(sizeof(struct list_head) * objects, GFP_KERNEL);
	unsigned long flags;

	if (!slabs_by_inuse)
		return -ENOMEM;

	flush_all(s);
	for_each_node_state(node, N_NORMAL_MEMORY) {
		n = get_node(s, node);

		if (!n->nr_partial)
			continue;

		for (i = 0; i < objects; i++)
			INIT_LIST_HEAD(slabs_by_inuse + i);

		spin_lock_irqsave(&n->list_lock, flags);

		 
		list_for_each_entry_safe(page, t, &n->partial, lru) {
			if (!page->inuse && slab_trylock(page)) {
				 
				list_del(&page->lru);
				n->nr_partial--;
				slab_unlock(page);
				discard_slab(s, page);
			} else {
				list_move(&page->lru,
				slabs_by_inuse + page->inuse);
			}
		}

		 
		for (i = objects - 1; i >= 0; i--)
			list_splice(slabs_by_inuse + i, n->partial.prev);

		spin_unlock_irqrestore(&n->list_lock, flags);
	}

	kfree(slabs_by_inuse);
	return 0;
}
