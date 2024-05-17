static bool shuffle_freelist(struct kmem_cache *cachep, struct page *page)
{
	unsigned int objfreelist = 0, i, rand, count = cachep->num;
	union freelist_init_state state;
	bool precomputed;

	if (count < 2)
		return false;

	precomputed = freelist_state_initialize(&state, cachep, count);

	 
	if (OBJFREELIST_SLAB(cachep)) {
		if (!precomputed)
			objfreelist = count - 1;
		else
			objfreelist = next_random_slot(&state);
		page->freelist = index_to_obj(cachep, page, objfreelist) +
						obj_offset(cachep);
		count--;
	}

	 
	if (!precomputed) {
		for (i = 0; i < count; i++)
			set_free_obj(page, i, i);

		 
		for (i = count - 1; i > 0; i--) {
			rand = prandom_u32_state(&state.rnd_state);
			rand %= (i + 1);
			swap_free_obj(page, i, rand);
		}
	} else {
		for (i = 0; i < count; i++)
			set_free_obj(page, i, next_random_slot(&state));
	}

	if (OBJFREELIST_SLAB(cachep))
		set_free_obj(page, cachep->num - 1, objfreelist);

	return true;
}
