static void *fallback_alloc(struct kmem_cache *cache, gfp_t flags)
{
	struct zonelist *zonelist;
	struct zoneref *z;
	struct zone *zone;
	enum zone_type high_zoneidx = gfp_zone(flags);
	void *obj = NULL;
	struct page *page;
	int nid;
	unsigned int cpuset_mems_cookie;

	if (flags & __GFP_THISNODE)
		return NULL;

retry_cpuset:
	cpuset_mems_cookie = read_mems_allowed_begin();
	zonelist = node_zonelist(mempolicy_slab_node(), flags);

retry:
	 
	for_each_zone_zonelist(zone, z, zonelist, high_zoneidx) {
		nid = zone_to_nid(zone);

		if (cpuset_zone_allowed(zone, flags) &&
			get_node(cache, nid) &&
			get_node(cache, nid)->free_objects) {
				obj = ____cache_alloc_node(cache,
					gfp_exact_node(flags), nid);
				if (obj)
					break;
		}
	}

	if (!obj) {
		 
		page = cache_grow_begin(cache, flags, numa_mem_id());
		cache_grow_end(cache, page);
		if (page) {
			nid = page_to_nid(page);
			obj = ____cache_alloc_node(cache,
				gfp_exact_node(flags), nid);

			 
			if (!obj)
				goto retry;
		}
	}

	if (unlikely(!obj && read_mems_allowed_retry(cpuset_mems_cookie)))
		goto retry_cpuset;
	return obj;
}
