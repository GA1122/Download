static size_t calculate_slab_order(struct kmem_cache *cachep,
				size_t size, unsigned long flags)
{
	size_t left_over = 0;
	int gfporder;

	for (gfporder = 0; gfporder <= KMALLOC_MAX_ORDER; gfporder++) {
		unsigned int num;
		size_t remainder;

		num = cache_estimate(gfporder, size, flags, &remainder);
		if (!num)
			continue;

		 
		if (num > SLAB_OBJ_MAX_NUM)
			break;

		if (flags & CFLGS_OFF_SLAB) {
			struct kmem_cache *freelist_cache;
			size_t freelist_size;

			freelist_size = num * sizeof(freelist_idx_t);
			freelist_cache = kmalloc_slab(freelist_size, 0u);
			if (!freelist_cache)
				continue;

			 
			if (OFF_SLAB(freelist_cache))
				continue;

			 
			if (freelist_cache->size > cachep->size / 2)
				continue;
		}

		 
		cachep->num = num;
		cachep->gfporder = gfporder;
		left_over = remainder;

		 
		if (flags & SLAB_RECLAIM_ACCOUNT)
			break;

		 
		if (gfporder >= slab_max_order)
			break;

		 
		if (left_over * 8 <= (PAGE_SIZE << gfporder))
			break;
	}
	return left_over;
}
