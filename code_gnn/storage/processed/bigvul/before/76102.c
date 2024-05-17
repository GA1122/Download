get_free_alloc_entry(void)
{
	MEMCHECK *entry;

	 
	if (free_list_size < 256)
		entry = malloc(sizeof *entry);
	else {
		entry = list_first_entry(&free_list, MEMCHECK, l);
		list_head_del(&entry->l);
		free_list_size--;
	}

	entry->seq_num = seq_num++;

	return entry;
}