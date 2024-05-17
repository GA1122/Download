static void *eval_map_next(struct seq_file *m, void *v, loff_t *pos)
{
	union trace_eval_map_item *ptr = v;

	 
	ptr = update_eval_map(ptr);
	if (WARN_ON_ONCE(!ptr))
		return NULL;

	ptr++;

	(*pos)++;

	ptr = update_eval_map(ptr);

	return ptr;
}
