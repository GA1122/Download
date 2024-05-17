__kmem_cache_create (struct kmem_cache *cachep, unsigned long flags)
{
	size_t ralign = BYTES_PER_WORD;
	gfp_t gfp;
	int err;
	size_t size = cachep->size;

#if DEBUG
#if FORCED_DEBUG
	 
	if (size < 4096 || fls(size - 1) == fls(size-1 + REDZONE_ALIGN +
						2 * sizeof(unsigned long long)))
		flags |= SLAB_RED_ZONE | SLAB_STORE_USER;
	if (!(flags & SLAB_DESTROY_BY_RCU))
		flags |= SLAB_POISON;
#endif
#endif

	 
	if (size & (BYTES_PER_WORD - 1)) {
		size += (BYTES_PER_WORD - 1);
		size &= ~(BYTES_PER_WORD - 1);
	}

	if (flags & SLAB_RED_ZONE) {
		ralign = REDZONE_ALIGN;
		 
		size += REDZONE_ALIGN - 1;
		size &= ~(REDZONE_ALIGN - 1);
	}

	 
	if (ralign < cachep->align) {
		ralign = cachep->align;
	}
	 
	if (ralign > __alignof__(unsigned long long))
		flags &= ~(SLAB_RED_ZONE | SLAB_STORE_USER);
	 
	cachep->align = ralign;
	cachep->colour_off = cache_line_size();
	 
	if (cachep->colour_off < cachep->align)
		cachep->colour_off = cachep->align;

	if (slab_is_available())
		gfp = GFP_KERNEL;
	else
		gfp = GFP_NOWAIT;

#if DEBUG

	 
	if (flags & SLAB_RED_ZONE) {
		 
		cachep->obj_offset += sizeof(unsigned long long);
		size += 2 * sizeof(unsigned long long);
	}
	if (flags & SLAB_STORE_USER) {
		 
		if (flags & SLAB_RED_ZONE)
			size += REDZONE_ALIGN;
		else
			size += BYTES_PER_WORD;
	}
#endif

	kasan_cache_create(cachep, &size, &flags);

	size = ALIGN(size, cachep->align);
	 
	if (FREELIST_BYTE_INDEX && size < SLAB_OBJ_MIN_SIZE)
		size = ALIGN(SLAB_OBJ_MIN_SIZE, cachep->align);

#if DEBUG
	 
	if (debug_pagealloc_enabled() && (flags & SLAB_POISON) &&
		size >= 256 && cachep->object_size > cache_line_size()) {
		if (size < PAGE_SIZE || size % PAGE_SIZE == 0) {
			size_t tmp_size = ALIGN(size, PAGE_SIZE);

			if (set_off_slab_cache(cachep, tmp_size, flags)) {
				flags |= CFLGS_OFF_SLAB;
				cachep->obj_offset += tmp_size - size;
				size = tmp_size;
				goto done;
			}
		}
	}
#endif

	if (set_objfreelist_slab_cache(cachep, size, flags)) {
		flags |= CFLGS_OBJFREELIST_SLAB;
		goto done;
	}

	if (set_off_slab_cache(cachep, size, flags)) {
		flags |= CFLGS_OFF_SLAB;
		goto done;
	}

	if (set_on_slab_cache(cachep, size, flags))
		goto done;

	return -E2BIG;

done:
	cachep->freelist_size = cachep->num * sizeof(freelist_idx_t);
	cachep->flags = flags;
	cachep->allocflags = __GFP_COMP;
	if (flags & SLAB_CACHE_DMA)
		cachep->allocflags |= GFP_DMA;
	cachep->size = size;
	cachep->reciprocal_buffer_size = reciprocal_value(size);

#if DEBUG
	 
	if (IS_ENABLED(CONFIG_PAGE_POISONING) &&
		(cachep->flags & SLAB_POISON) &&
		is_debug_pagealloc_cache(cachep))
		cachep->flags &= ~(SLAB_RED_ZONE | SLAB_STORE_USER);
#endif

	if (OFF_SLAB(cachep)) {
		cachep->freelist_cache =
			kmalloc_slab(cachep->freelist_size, 0u);
	}

	err = setup_cpu_cache(cachep, gfp);
	if (err) {
		__kmem_cache_release(cachep);
		return err;
	}

	return 0;
}
