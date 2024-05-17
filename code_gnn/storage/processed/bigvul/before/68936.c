static void slab_destroy_debugcheck(struct kmem_cache *cachep,
						struct page *page)
{
	int i;

	if (OBJFREELIST_SLAB(cachep) && cachep->flags & SLAB_POISON) {
		poison_obj(cachep, page->freelist - obj_offset(cachep),
			POISON_FREE);
	}

	for (i = 0; i < cachep->num; i++) {
		void *objp = index_to_obj(cachep, page, i);

		if (cachep->flags & SLAB_POISON) {
			check_poison_obj(cachep, objp);
			slab_kernel_map(cachep, objp, 1, 0);
		}
		if (cachep->flags & SLAB_RED_ZONE) {
			if (*dbg_redzone1(cachep, objp) != RED_INACTIVE)
				slab_error(cachep, "start of a freed object was overwritten");
			if (*dbg_redzone2(cachep, objp) != RED_INACTIVE)
				slab_error(cachep, "end of a freed object was overwritten");
		}
	}
}
