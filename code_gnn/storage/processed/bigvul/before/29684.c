static void read_exception(struct pstore *ps,
			   uint32_t index, struct core_exception *result)
{
	struct disk_exception *de = get_exception(ps, index);

	 
	result->old_chunk = le64_to_cpu(de->old_chunk);
	result->new_chunk = le64_to_cpu(de->new_chunk);
}