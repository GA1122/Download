static bool is_racy_entry(git_index *index, const git_index_entry *entry)
{
	 
	if (S_ISGITLINK(entry->mode))
		return false;

	return git_index_entry_newer_than_index(entry, index);
}