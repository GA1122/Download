static int persistent_read_metadata(struct dm_exception_store *store,
				    int (*callback)(void *callback_context,
						    chunk_t old, chunk_t new),
				    void *callback_context)
{
	int r, uninitialized_var(new_snapshot);
	struct pstore *ps = get_info(store);

	 
	r = read_header(ps, &new_snapshot);
	if (r)
		return r;

	 
	ps->exceptions_per_area = (ps->store->chunk_size << SECTOR_SHIFT) /
				  sizeof(struct disk_exception);
	ps->callbacks = dm_vcalloc(ps->exceptions_per_area,
				   sizeof(*ps->callbacks));
	if (!ps->callbacks)
		return -ENOMEM;

	 
	if (new_snapshot) {
		r = write_header(ps);
		if (r) {
			DMWARN("write_header failed");
			return r;
		}

		ps->current_area = 0;
		zero_memory_area(ps);
		r = zero_disk_area(ps, 0);
		if (r)
			DMWARN("zero_disk_area(0) failed");
		return r;
	}
	 
	if (ps->version != SNAPSHOT_DISK_VERSION) {
		DMWARN("unable to handle snapshot disk version %d",
		       ps->version);
		return -EINVAL;
	}

	 
	if (!ps->valid)
		return 1;

	 
	r = read_exceptions(ps, callback, callback_context);

	return r;
}