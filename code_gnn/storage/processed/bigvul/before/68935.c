static void slab_destroy(struct kmem_cache *cachep, struct page *page)
{
	void *freelist;

	freelist = page->freelist;
	slab_destroy_debugcheck(cachep, page);
	if (unlikely(cachep->flags & SLAB_DESTROY_BY_RCU))
		call_rcu(&page->rcu_head, kmem_rcu_free);
	else
		kmem_freepages(cachep, page);

	 
	if (OFF_SLAB(cachep))
		kmem_cache_free(cachep->freelist_cache, freelist);
}