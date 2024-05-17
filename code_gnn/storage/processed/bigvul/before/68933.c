slab_alloc_node(struct kmem_cache *cachep, gfp_t flags, int nodeid,
		   unsigned long caller)
{
	unsigned long save_flags;
	void *ptr;
	int slab_node = numa_mem_id();

	flags &= gfp_allowed_mask;
	cachep = slab_pre_alloc_hook(cachep, flags);
	if (unlikely(!cachep))
		return NULL;

	cache_alloc_debugcheck_before(cachep, flags);
	local_irq_save(save_flags);

	if (nodeid == NUMA_NO_NODE)
		nodeid = slab_node;

	if (unlikely(!get_node(cachep, nodeid))) {
		 
		ptr = fallback_alloc(cachep, flags);
		goto out;
	}

	if (nodeid == slab_node) {
		 
		ptr = ____cache_alloc(cachep, flags);
		if (ptr)
			goto out;
	}
	 
	ptr = ____cache_alloc_node(cachep, flags, nodeid);
  out:
	local_irq_restore(save_flags);
	ptr = cache_alloc_debugcheck_after(cachep, flags, ptr, caller);

	if (unlikely(flags & __GFP_ZERO) && ptr)
		memset(ptr, 0, cachep->object_size);

	slab_post_alloc_hook(cachep, flags, 1, &ptr);
	return ptr;
}
