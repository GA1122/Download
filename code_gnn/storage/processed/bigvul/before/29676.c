static int persistent_commit_merge(struct dm_exception_store *store,
				   int nr_merged)
{
	int r, i;
	struct pstore *ps = get_info(store);

	BUG_ON(nr_merged > ps->current_committed);

	for (i = 0; i < nr_merged; i++)
		clear_exception(ps, ps->current_committed - 1 - i);

	r = area_io(ps, WRITE_FLUSH_FUA);
	if (r < 0)
		return r;

	ps->current_committed -= nr_merged;

	 
	ps->next_free = area_location(ps, ps->current_area) +
			ps->current_committed + 1;

	return 0;
}
