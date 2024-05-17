static int should_defrag_range(struct inode *inode, u64 start, int thresh,
			       u64 *last_len, u64 *skip, u64 *defrag_end,
			       int compress)
{
	struct extent_map *em;
	int ret = 1;
	bool next_mergeable = true;

	 
	if (start < *defrag_end)
		return 1;

	*skip = 0;

	em = defrag_lookup_extent(inode, start);
	if (!em)
		return 0;

	 
	if (em->block_start >= EXTENT_MAP_LAST_BYTE) {
		ret = 0;
		goto out;
	}

	next_mergeable = defrag_check_next_extent(inode, em);

	 
	if (!compress && (*last_len == 0 || *last_len >= thresh) &&
	    (em->len >= thresh || !next_mergeable))
		ret = 0;
out:
	 
	if (ret) {
		*defrag_end = extent_map_end(em);
	} else {
		*last_len = 0;
		*skip = extent_map_end(em);
		*defrag_end = 0;
	}

	free_extent_map(em);
	return ret;
}
