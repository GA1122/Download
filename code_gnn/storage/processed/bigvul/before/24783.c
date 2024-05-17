static void deactivate_slab(struct kmem_cache *s, struct kmem_cache_cpu *c)
{
	struct page *page = c->page;
	int tail = 1;

	if (page->freelist)
		stat(c, DEACTIVATE_REMOTE_FREES);
	 
	while (unlikely(c->freelist)) {
		void **object;

		tail = 0;	 

		 
		object = c->freelist;
		c->freelist = c->freelist[c->offset];

		 
		object[c->offset] = page->freelist;
		page->freelist = object;
		page->inuse--;
	}
	c->page = NULL;
	unfreeze_slab(s, page, tail);
}
