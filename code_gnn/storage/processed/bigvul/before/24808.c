static struct page *get_partial_node(struct kmem_cache_node *n)
{
	struct page *page;

	 
	if (!n || !n->nr_partial)
		return NULL;

	spin_lock(&n->list_lock);
	list_for_each_entry(page, &n->partial, lru)
		if (lock_and_freeze_slab(n, page))
			goto out;
	page = NULL;
out:
	spin_unlock(&n->list_lock);
	return page;
}
