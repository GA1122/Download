static int persistent_prepare_merge(struct dm_exception_store *store,
				    chunk_t *last_old_chunk,
				    chunk_t *last_new_chunk)
{
	struct pstore *ps = get_info(store);
	struct core_exception ce;
	int nr_consecutive;
	int r;

	 
	if (!ps->current_committed) {
		 
		if (!ps->current_area)
			return 0;

		ps->current_area--;
		r = area_io(ps, READ);
		if (r < 0)
			return r;
		ps->current_committed = ps->exceptions_per_area;
	}

	read_exception(ps, ps->current_committed - 1, &ce);
	*last_old_chunk = ce.old_chunk;
	*last_new_chunk = ce.new_chunk;

	 
	for (nr_consecutive = 1; nr_consecutive < ps->current_committed;
	     nr_consecutive++) {
		read_exception(ps, ps->current_committed - 1 - nr_consecutive,
			       &ce);
		if (ce.old_chunk != *last_old_chunk - nr_consecutive ||
		    ce.new_chunk != *last_new_chunk - nr_consecutive)
			break;
	}

	return nr_consecutive;
}