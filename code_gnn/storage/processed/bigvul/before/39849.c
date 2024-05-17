static void *__kmalloc_reserve(size_t size, gfp_t flags, int node,
			       unsigned long ip, bool *pfmemalloc)
{
	void *obj;
	bool ret_pfmemalloc = false;

	 
	obj = kmalloc_node_track_caller(size,
					flags | __GFP_NOMEMALLOC | __GFP_NOWARN,
					node);
	if (obj || !(gfp_pfmemalloc_allowed(flags)))
		goto out;

	 
	ret_pfmemalloc = true;
	obj = kmalloc_node_track_caller(size, flags, node);

out:
	if (pfmemalloc)
		*pfmemalloc = ret_pfmemalloc;

	return obj;
}
