static void prepare_pack(int window, int depth)
{
	struct object_entry **delta_list;
	uint32_t i, nr_deltas;
	unsigned n;

	get_object_details();

	 
	if (!pack_to_stdout)
		do_check_packed_object_crc = 1;

	if (!to_pack.nr_objects || !window || !depth)
		return;

	delta_list = xmalloc(to_pack.nr_objects * sizeof(*delta_list));
	nr_deltas = n = 0;

	for (i = 0; i < to_pack.nr_objects; i++) {
		struct object_entry *entry = to_pack.objects + i;

		if (entry->delta)
			 
			continue;

		if (entry->size < 50)
			continue;

		if (entry->no_try_delta)
			continue;

		if (!entry->preferred_base) {
			nr_deltas++;
			if (entry->type < 0)
				die("unable to get type of object %s",
				    sha1_to_hex(entry->idx.sha1));
		} else {
			if (entry->type < 0) {
				 
				continue;
			}
		}

		delta_list[n++] = entry;
	}

	if (nr_deltas && n > 1) {
		unsigned nr_done = 0;
		if (progress)
			progress_state = start_progress(_("Compressing objects"),
							nr_deltas);
		qsort(delta_list, n, sizeof(*delta_list), type_size_sort);
		ll_find_deltas(delta_list, n, window+1, depth, &nr_done);
		stop_progress(&progress_state);
		if (nr_done != nr_deltas)
			die("inconsistency with delta count");
	}
	free(delta_list);
}
