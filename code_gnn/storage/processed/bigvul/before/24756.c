static int alloc_debug_processing(struct kmem_cache *s, struct page *page,
						void *object, void *addr)
{
	if (!check_slab(s, page))
		goto bad;

	if (!on_freelist(s, page, object)) {
		object_err(s, page, object, "Object already allocated");
		goto bad;
	}

	if (!check_valid_pointer(s, page, object)) {
		object_err(s, page, object, "Freelist Pointer check fails");
		goto bad;
	}

	if (!check_object(s, page, object, 0))
		goto bad;

	 
	if (s->flags & SLAB_STORE_USER)
		set_track(s, object, TRACK_ALLOC, addr);
	trace(s, page, object, 1);
	init_object(s, object, 1);
	return 1;

bad:
	if (PageSlab(page)) {
		 
		slab_fix(s, "Marking all objects used");
		page->inuse = page->objects;
		page->freelist = NULL;
	}
	return 0;
}