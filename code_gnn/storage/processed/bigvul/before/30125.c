enter_record(struct ftrace_hash *hash, struct dyn_ftrace *rec, int not)
{
	struct ftrace_func_entry *entry;
	int ret = 0;

	entry = ftrace_lookup_ip(hash, rec->ip);
	if (not) {
		 
		if (!entry)
			return 0;

		free_hash_entry(hash, entry);
	} else {
		 
		if (entry)
			return 0;

		ret = add_hash_entry(hash, rec->ip);
	}
	return ret;
}
