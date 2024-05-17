static int should_include(struct commit *commit, void *_data)
{
	struct include_data *data = _data;
	int bitmap_pos;

	bitmap_pos = bitmap_position(commit->object.oid.hash);
	if (bitmap_pos < 0)
		bitmap_pos = ext_index_add_object((struct object *)commit, NULL);

	if (!add_to_include_set(data, commit->object.oid.hash, bitmap_pos)) {
		struct commit_list *parent = commit->parents;

		while (parent) {
			parent->item->object.flags |= SEEN;
			parent = parent->next;
		}

		return 0;
	}

	return 1;
}
