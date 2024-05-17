static int drain_freelist(struct kmem_cache *cache,
			struct kmem_cache_node *n, int tofree)
{
	struct list_head *p;
	int nr_freed;
	struct page *page;

	nr_freed = 0;
	while (nr_freed < tofree && !list_empty(&n->slabs_free)) {

		spin_lock_irq(&n->list_lock);
		p = n->slabs_free.prev;
		if (p == &n->slabs_free) {
			spin_unlock_irq(&n->list_lock);
			goto out;
		}

		page = list_entry(p, struct page, lru);
		list_del(&page->lru);
		n->free_slabs--;
		n->total_slabs--;
		 
		n->free_objects -= cache->num;
		spin_unlock_irq(&n->list_lock);
		slab_destroy(cache, page);
		nr_freed++;
	}
out:
	return nr_freed;
}