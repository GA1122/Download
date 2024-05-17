static void free_block(struct kmem_cache *cachep, void **objpp,
			int nr_objects, int node, struct list_head *list)
{
	int i;
	struct kmem_cache_node *n = get_node(cachep, node);
	struct page *page;

	n->free_objects += nr_objects;

	for (i = 0; i < nr_objects; i++) {
		void *objp;
		struct page *page;

		objp = objpp[i];

		page = virt_to_head_page(objp);
		list_del(&page->lru);
		check_spinlock_acquired_node(cachep, node);
		slab_put_obj(cachep, page, objp);
		STATS_DEC_ACTIVE(cachep);

		 
		if (page->active == 0) {
			list_add(&page->lru, &n->slabs_free);
			n->free_slabs++;
		} else {
			 
			list_add_tail(&page->lru, &n->slabs_partial);
		}
	}

	while (n->free_objects > n->free_limit && !list_empty(&n->slabs_free)) {
		n->free_objects -= cachep->num;

		page = list_last_entry(&n->slabs_free, struct page, lru);
		list_move(&page->lru, list);
		n->free_slabs--;
		n->total_slabs--;
	}
}