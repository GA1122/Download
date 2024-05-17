int prepare_bitmap_walk(struct rev_info *revs)
{
	unsigned int i;
	unsigned int pending_nr = revs->pending.nr;
	struct object_array_entry *pending_e = revs->pending.objects;

	struct object_list *wants = NULL;
	struct object_list *haves = NULL;

	struct bitmap *wants_bitmap = NULL;
	struct bitmap *haves_bitmap = NULL;

	if (!bitmap_git.loaded) {
		 
		if (open_pack_bitmap() < 0)
			return -1;
	}

	for (i = 0; i < pending_nr; ++i) {
		struct object *object = pending_e[i].item;

		if (object->type == OBJ_NONE)
			parse_object_or_die(object->oid.hash, NULL);

		while (object->type == OBJ_TAG) {
			struct tag *tag = (struct tag *) object;

			if (object->flags & UNINTERESTING)
				object_list_insert(object, &haves);
			else
				object_list_insert(object, &wants);

			if (!tag->tagged)
				die("bad tag");
			object = parse_object_or_die(tag->tagged->oid.hash, NULL);
		}

		if (object->flags & UNINTERESTING)
			object_list_insert(object, &haves);
		else
			object_list_insert(object, &wants);
	}

	 
	if (haves && !in_bitmapped_pack(haves))
		return -1;

	 
	if (!wants)
		return -1;

	 
	if (!bitmap_git.loaded && load_pack_bitmap() < 0)
		return -1;

	revs->pending.nr = 0;
	revs->pending.alloc = 0;
	revs->pending.objects = NULL;

	if (haves) {
		revs->ignore_missing_links = 1;
		haves_bitmap = find_objects(revs, haves, NULL);
		reset_revision_walk();
		revs->ignore_missing_links = 0;

		if (haves_bitmap == NULL)
			die("BUG: failed to perform bitmap walk");
	}

	wants_bitmap = find_objects(revs, wants, haves_bitmap);

	if (!wants_bitmap)
		die("BUG: failed to perform bitmap walk");

	if (haves_bitmap)
		bitmap_and_not(wants_bitmap, haves_bitmap);

	bitmap_git.result = wants_bitmap;

	bitmap_free(haves_bitmap);
	return 0;
}
