static void index_free(git_index *index)
{
	 
	assert(!git_atomic_get(&index->readers));

	git_index_clear(index);
	git_idxmap_free(index->entries_map);
	git_vector_free(&index->entries);
	git_vector_free(&index->names);
	git_vector_free(&index->reuc);
	git_vector_free(&index->deleted);

	git__free(index->index_file_path);

	git__memzero(index, sizeof(*index));
	git__free(index);
}
