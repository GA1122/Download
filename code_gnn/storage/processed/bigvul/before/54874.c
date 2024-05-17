static void show_commit(struct commit *commit, void *data)
{
	add_object_entry(commit->object.oid.hash, OBJ_COMMIT, NULL, 0);
	commit->object.flags |= OBJECT_ADDED;

	if (write_bitmap_index)
 		index_commit_for_bitmap(commit);
 }
