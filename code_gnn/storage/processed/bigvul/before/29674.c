static int insert_exceptions(struct pstore *ps,
			     int (*callback)(void *callback_context,
					     chunk_t old, chunk_t new),
			     void *callback_context,
			     int *full)
{
	int r;
	unsigned int i;
	struct core_exception e;

	 
	*full = 1;

	for (i = 0; i < ps->exceptions_per_area; i++) {
		read_exception(ps, i, &e);

		 
		if (e.new_chunk == 0LL) {
			ps->current_committed = i;
			*full = 0;
			break;
		}

		 
		if (ps->next_free <= e.new_chunk)
			ps->next_free = e.new_chunk + 1;

		 
		r = callback(callback_context, e.old_chunk, e.new_chunk);
		if (r)
			return r;
	}

	return 0;
}
