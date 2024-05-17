int kmem_cache_alloc_bulk(struct kmem_cache *s, gfp_t flags, size_t size,
			  void **p)
{
	size_t i;

	s = slab_pre_alloc_hook(s, flags);
	if (!s)
		return 0;

	cache_alloc_debugcheck_before(s, flags);

	local_irq_disable();
	for (i = 0; i < size; i++) {
		void *objp = __do_cache_alloc(s, flags);

		if (unlikely(!objp))
			goto error;
		p[i] = objp;
	}
	local_irq_enable();

	cache_alloc_debugcheck_after_bulk(s, flags, size, p, _RET_IP_);

	 
	if (unlikely(flags & __GFP_ZERO))
		for (i = 0; i < size; i++)
			memset(p[i], 0, s->object_size);

	slab_post_alloc_hook(s, flags, size, p);
	 
	return size;
error:
	local_irq_enable();
	cache_alloc_debugcheck_after_bulk(s, flags, i, p, _RET_IP_);
	slab_post_alloc_hook(s, flags, i, p);
	__kmem_cache_free_bulk(s, i, p);
	return 0;
}