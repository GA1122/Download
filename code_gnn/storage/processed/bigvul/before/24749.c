static void add_full(struct kmem_cache_node *n, struct page *page)
{
	spin_lock(&n->list_lock);
	list_add(&page->lru, &n->full);
	spin_unlock(&n->list_lock);
}
