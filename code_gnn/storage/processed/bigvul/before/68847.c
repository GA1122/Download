static struct page *cache_grow_begin(struct kmem_cache *cachep,
				gfp_t flags, int nodeid)
{
	void *freelist;
	size_t offset;
	gfp_t local_flags;
	int page_node;
	struct kmem_cache_node *n;
	struct page *page;

	 
	if (unlikely(flags & GFP_SLAB_BUG_MASK)) {
		gfp_t invalid_mask = flags & GFP_SLAB_BUG_MASK;
		flags &= ~GFP_SLAB_BUG_MASK;
		pr_warn("Unexpected gfp: %#x (%pGg). Fixing up to gfp: %#x (%pGg). Fix your code!\n",
				invalid_mask, &invalid_mask, flags, &flags);
		dump_stack();
	}
	local_flags = flags & (GFP_CONSTRAINT_MASK|GFP_RECLAIM_MASK);

	check_irq_off();
	if (gfpflags_allow_blocking(local_flags))
		local_irq_enable();

	 
	page = kmem_getpages(cachep, local_flags, nodeid);
	if (!page)
		goto failed;

	page_node = page_to_nid(page);
	n = get_node(cachep, page_node);

	 
	n->colour_next++;
	if (n->colour_next >= cachep->colour)
		n->colour_next = 0;

	offset = n->colour_next;
	if (offset >= cachep->colour)
		offset = 0;

	offset *= cachep->colour_off;

	 
	freelist = alloc_slabmgmt(cachep, page, offset,
			local_flags & ~GFP_CONSTRAINT_MASK, page_node);
	if (OFF_SLAB(cachep) && !freelist)
		goto opps1;

	slab_map_pages(cachep, page, freelist);

	kasan_poison_slab(page);
	cache_init_objs(cachep, page);

	if (gfpflags_allow_blocking(local_flags))
		local_irq_disable();

	return page;

opps1:
	kmem_freepages(cachep, page);
failed:
	if (gfpflags_allow_blocking(local_flags))
		local_irq_disable();
	return NULL;
}