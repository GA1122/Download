static void clear_exception(struct pstore *ps, uint32_t index)
{
	struct disk_exception *de = get_exception(ps, index);

	 
	de->old_chunk = 0;
	de->new_chunk = 0;
}