static inline int cache_free_alien(struct kmem_cache *cachep, void *objp)
{
	int page_node = page_to_nid(virt_to_page(objp));
	int node = numa_mem_id();
	 
	if (likely(node == page_node))
		return 0;

	return __cache_free_alien(cachep, objp, node, page_node);
}
