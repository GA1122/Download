static struct page *get_any_partial(struct kmem_cache *s, gfp_t flags)
{
#ifdef CONFIG_NUMA
	struct zonelist *zonelist;
	struct zoneref *z;
	struct zone *zone;
	enum zone_type high_zoneidx = gfp_zone(flags);
	struct page *page;

	 
	if (!s->remote_node_defrag_ratio ||
			get_cycles() % 1024 > s->remote_node_defrag_ratio)
		return NULL;

	zonelist = node_zonelist(slab_node(current->mempolicy), flags);
	for_each_zone_zonelist(zone, z, zonelist, high_zoneidx) {
		struct kmem_cache_node *n;

		n = get_node(s, zone_to_nid(zone));

		if (n && cpuset_zone_allowed_hardwall(zone, flags) &&
				n->nr_partial > MIN_PARTIAL) {
			page = get_partial_node(n);
			if (page)
				return page;
		}
	}
#endif
	return NULL;
}
