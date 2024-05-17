const char *__check_heap_object(const void *ptr, unsigned long n,
				struct page *page)
{
	struct kmem_cache *cachep;
	unsigned int objnr;
	unsigned long offset;

	 
	cachep = page->slab_cache;
	objnr = obj_to_index(cachep, page, (void *)ptr);
	BUG_ON(objnr >= cachep->num);

	 
	offset = ptr - index_to_obj(cachep, page, objnr) - obj_offset(cachep);

	 
	if (offset <= cachep->object_size && n <= cachep->object_size - offset)
		return NULL;

	return cachep->name;
}
