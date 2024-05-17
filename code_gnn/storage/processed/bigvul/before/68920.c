static bool set_off_slab_cache(struct kmem_cache *cachep,
			size_t size, unsigned long flags)
{
	size_t left;

	cachep->num = 0;

	 
	if (flags & SLAB_NOLEAKTRACE)
		return false;

	 
	left = calculate_slab_order(cachep, size, flags | CFLGS_OFF_SLAB);
	if (!cachep->num)
		return false;

	 
	if (left >= cachep->num * sizeof(freelist_idx_t))
		return false;

	cachep->colour = left / cachep->colour_off;

	return true;
}
