static void persistent_commit_exception(struct dm_exception_store *store,
					struct dm_exception *e,
					void (*callback) (void *, int success),
					void *callback_context)
{
	unsigned int i;
	struct pstore *ps = get_info(store);
	struct core_exception ce;
	struct commit_callback *cb;

	ce.old_chunk = e->old_chunk;
	ce.new_chunk = e->new_chunk;
	write_exception(ps, ps->current_committed++, &ce);

	 
	cb = ps->callbacks + ps->callback_count++;
	cb->callback = callback;
	cb->context = callback_context;

	 
	if (!atomic_dec_and_test(&ps->pending_count) &&
	    (ps->current_committed != ps->exceptions_per_area))
		return;

	 
	if ((ps->current_committed == ps->exceptions_per_area) &&
	    zero_disk_area(ps, ps->current_area + 1))
		ps->valid = 0;

	 
	if (ps->valid && area_io(ps, WRITE_FLUSH_FUA))
		ps->valid = 0;

	 
	if (ps->current_committed == ps->exceptions_per_area) {
		ps->current_committed = 0;
		ps->current_area++;
		zero_memory_area(ps);
	}

	for (i = 0; i < ps->callback_count; i++) {
		cb = ps->callbacks + i;
		cb->callback(cb->context, ps->valid);
	}

	ps->callback_count = 0;
}
