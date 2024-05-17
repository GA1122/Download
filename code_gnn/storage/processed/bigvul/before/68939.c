static void slab_kernel_map(struct kmem_cache *cachep, void *objp,
				int map, unsigned long caller)
{
	if (!is_debug_pagealloc_cache(cachep))
		return;

	if (caller)
		store_stackinfo(cachep, objp, caller);

	kernel_map_pages(virt_to_page(objp), cachep->size / PAGE_SIZE, map);
}
