keepalived_malloc(size_t size, const char *file, const char *function, int line)
{
	void *buf;
	MEMCHECK *entry, *entry2;

	buf = zalloc(size + sizeof (unsigned long));

	*(unsigned long *) ((char *) buf + size) = size + CHECK_VAL;

	entry = get_free_alloc_entry();

	entry->ptr = buf;
	entry->size = size;
	entry->file = file;
	entry->func = function;
	entry->line = line;
	entry->type = ALLOCATED;

	rb_insert_sort(&alloc_list, entry, t, memcheck_ptr_cmp);
	if (++number_alloc_list > max_alloc_list)
		max_alloc_list = number_alloc_list;

	fprintf(log_op, "%szalloc [%3d:%3d], %9p, %4zu at %s, %3d, %s%s\n",
	       format_time(), entry->seq_num, number_alloc_list, buf, size, file, line, function, !size ? " - size is 0" : "");
#ifdef _MEM_CHECK_LOG_
	if (__test_bit(MEM_CHECK_LOG_BIT, &debug))
		log_message(LOG_INFO, "zalloc[%3d:%3d], %9p, %4zu at %s, %3d, %s",
		       entry->seq_num, number_alloc_list, buf, size, file, line, function);
#endif

	num_mallocs++;

	if (!size) {
		 
		entry2 = get_free_alloc_entry();
		*entry2 = *entry;
		entry2->type = MALLOC_ZERO_SIZE;
		list_add_tail(&entry2->l, &bad_list);
	}

	return buf;
}