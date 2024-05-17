size_t ksize(const void *object)
{
	struct page *page;
	struct kmem_cache *s;

	if (unlikely(object == ZERO_SIZE_PTR))
		return 0;

	page = virt_to_head_page(object);

	if (unlikely(!PageSlab(page)))
		return PAGE_SIZE << compound_order(page);

	s = page->slab;

#ifdef CONFIG_SLUB_DEBUG
	 
	if (s->flags & (SLAB_RED_ZONE | SLAB_POISON))
		return s->objsize;

#endif
	 
	if (s->flags & (SLAB_DESTROY_BY_RCU | SLAB_STORE_USER))
		return s->inuse;
	 
	return s->size;
}
