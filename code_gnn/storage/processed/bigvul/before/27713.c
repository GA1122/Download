static int size_entry_mwt(struct ebt_entry *entry, const unsigned char *base,
			  unsigned int *total,
			  struct ebt_entries_buf_state *state)
{
	unsigned int i, j, startoff, new_offset = 0;
	 
	unsigned int offsets[4];
	unsigned int *offsets_update = NULL;
	int ret;
	char *buf_start;

	if (*total < sizeof(struct ebt_entries))
		return -EINVAL;

	if (!entry->bitmask) {
		*total -= sizeof(struct ebt_entries);
		return ebt_buf_add(state, entry, sizeof(struct ebt_entries));
	}
	if (*total < sizeof(*entry) || entry->next_offset < sizeof(*entry))
		return -EINVAL;

	startoff = state->buf_user_offset;
	 
	ret = ebt_buf_add(state, entry,
			offsetof(struct ebt_entry, watchers_offset));
	if (ret < 0)
		return ret;

	offsets[0] = sizeof(struct ebt_entry);  
	memcpy(&offsets[1], &entry->watchers_offset,
			sizeof(offsets) - sizeof(offsets[0]));

	if (state->buf_kern_start) {
		buf_start = state->buf_kern_start + state->buf_kern_offset;
		offsets_update = (unsigned int *) buf_start;
	}
	ret = ebt_buf_add(state, &offsets[1],
			sizeof(offsets) - sizeof(offsets[0]));
	if (ret < 0)
		return ret;
	buf_start = (char *) entry;
	 
	for (i = 0, j = 1 ; j < 4 ; j++, i++) {
		struct compat_ebt_entry_mwt *match32;
		unsigned int size;
		char *buf = buf_start;

		buf = buf_start + offsets[i];
		if (offsets[i] > offsets[j])
			return -EINVAL;

		match32 = (struct compat_ebt_entry_mwt *) buf;
		size = offsets[j] - offsets[i];
		ret = ebt_size_mwt(match32, size, i, state, base);
		if (ret < 0)
			return ret;
		new_offset += ret;
		if (offsets_update && new_offset) {
			pr_debug("change offset %d to %d\n",
				offsets_update[i], offsets[j] + new_offset);
			offsets_update[i] = offsets[j] + new_offset;
		}
	}

	startoff = state->buf_user_offset - startoff;

	BUG_ON(*total < startoff);
	*total -= startoff;
	return 0;
}
