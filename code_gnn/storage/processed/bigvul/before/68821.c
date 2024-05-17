__kmem_cache_alias(const char *name, size_t size, size_t align,
		   unsigned long flags, void (*ctor)(void *))
{
	struct kmem_cache *cachep;

	cachep = find_mergeable(size, align, flags, name, ctor);
	if (cachep) {
		cachep->refcount++;

		 
		cachep->object_size = max_t(int, cachep->object_size, size);
	}
	return cachep;
}