static noinline struct page *get_valid_first_slab(struct kmem_cache_node *n,
					struct page *page, bool pfmemalloc)
{
	if (!page)
		return NULL;

	if (pfmemalloc)
		return page;

	if (!PageSlabPfmemalloc(page))
		return page;

	 
	if (n->free_objects > n->free_limit) {
		ClearPageSlabPfmemalloc(page);
		return page;
	}

	 
	list_del(&page->lru);
	if (!page->active) {
		list_add_tail(&page->lru, &n->slabs_free);
		n->free_slabs++;
	} else
		list_add_tail(&page->lru, &n->slabs_partial);

	list_for_each_entry(page, &n->slabs_partial, lru) {
		if (!PageSlabPfmemalloc(page))
			return page;
	}

	n->free_touched = 1;
	list_for_each_entry(page, &n->slabs_free, lru) {
		if (!PageSlabPfmemalloc(page)) {
			n->free_slabs--;
			return page;
		}
	}

	return NULL;
}