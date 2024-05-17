static unsigned long kmem_cache_flags(unsigned long objsize,
	unsigned long flags, const char *name,
	void (*ctor)(struct kmem_cache *, void *))
{
	 
	if (slub_debug && (!slub_debug_slabs ||
	    strncmp(slub_debug_slabs, name, strlen(slub_debug_slabs)) == 0))
			flags |= slub_debug;

	return flags;
}
