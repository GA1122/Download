__do_cache_alloc(struct kmem_cache *cache, gfp_t flags)
{
	void *objp;

	if (current->mempolicy || cpuset_do_slab_mem_spread()) {
		objp = alternate_node_alloc(cache, flags);
		if (objp)
			goto out;
	}
	objp = ____cache_alloc(cache, flags);

	 
	if (!objp)
		objp = ____cache_alloc_node(cache, flags, numa_mem_id());

  out:
	return objp;
}
