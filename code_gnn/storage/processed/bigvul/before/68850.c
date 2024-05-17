static void cache_init_objs_debug(struct kmem_cache *cachep, struct page *page)
{
#if DEBUG
	int i;

	for (i = 0; i < cachep->num; i++) {
		void *objp = index_to_obj(cachep, page, i);

		if (cachep->flags & SLAB_STORE_USER)
			*dbg_userword(cachep, objp) = NULL;

		if (cachep->flags & SLAB_RED_ZONE) {
			*dbg_redzone1(cachep, objp) = RED_INACTIVE;
			*dbg_redzone2(cachep, objp) = RED_INACTIVE;
		}
		 
		if (cachep->ctor && !(cachep->flags & SLAB_POISON)) {
			kasan_unpoison_object_data(cachep,
						   objp + obj_offset(cachep));
			cachep->ctor(objp + obj_offset(cachep));
			kasan_poison_object_data(
				cachep, objp + obj_offset(cachep));
		}

		if (cachep->flags & SLAB_RED_ZONE) {
			if (*dbg_redzone2(cachep, objp) != RED_INACTIVE)
				slab_error(cachep, "constructor overwrote the end of an object");
			if (*dbg_redzone1(cachep, objp) != RED_INACTIVE)
				slab_error(cachep, "constructor overwrote the start of an object");
		}
		 
		if (cachep->flags & SLAB_POISON) {
			poison_obj(cachep, objp, POISON_FREE);
			slab_kernel_map(cachep, objp, 0, 0);
		}
	}
#endif
}