static void *alloc_slabmgmt(struct kmem_cache *cachep,
				   struct page *page, int colour_off,
				   gfp_t local_flags, int nodeid)
{
	void *freelist;
	void *addr = page_address(page);

	page->s_mem = addr + colour_off;
	page->active = 0;

	if (OBJFREELIST_SLAB(cachep))
		freelist = NULL;
	else if (OFF_SLAB(cachep)) {
		 
		freelist = kmem_cache_alloc_node(cachep->freelist_cache,
					      local_flags, nodeid);
		if (!freelist)
			return NULL;
	} else {
		 
		freelist = addr + (PAGE_SIZE << cachep->gfporder) -
				cachep->freelist_size;
	}

	return freelist;
}