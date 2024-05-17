static inline void fixup_slab_list(struct kmem_cache *cachep,
				struct kmem_cache_node *n, struct page *page,
				void **list)
{
	 
	list_del(&page->lru);
	if (page->active == cachep->num) {
		list_add(&page->lru, &n->slabs_full);
		if (OBJFREELIST_SLAB(cachep)) {
#if DEBUG
			 
			if (cachep->flags & SLAB_POISON) {
				void **objp = page->freelist;

				*objp = *list;
				*list = objp;
			}
#endif
			page->freelist = NULL;
		}
	} else
		list_add(&page->lru, &n->slabs_partial);
}