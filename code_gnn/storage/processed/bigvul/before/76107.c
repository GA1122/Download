keepalived_free_realloc_common(void *buffer, size_t size, const char *file, const char *function, int line, bool is_realloc)
{
	unsigned long check;
	MEMCHECK *entry, *entry2, *le;
	MEMCHECK search = {.ptr = buffer};

	 
	if (buffer == NULL) {
		entry = get_free_alloc_entry();

		entry->ptr = NULL;
		entry->size = size;
		entry->file = file;
		entry->func = function;
		entry->line = line;
		entry->type = !size ? FREE_NULL : REALLOC_NULL;

		if (!is_realloc)
			fprintf(log_op, "%s%-7s%9s, %9s, %4s at %s, %3d, %s\n", format_time(),
				"free", "ERROR", "NULL", "",
				file, line, function);
		else
			fprintf(log_op, "%s%-7s%9s, %9s, %4zu at %s, %3d, %s%s\n", format_time(),
				"realloc", "ERROR", "NULL",
				size, file, line, function, size ? " *** converted to malloc" : "");

		__set_bit(MEM_ERR_DETECT_BIT, &debug);

		list_add_tail(&entry->l, &bad_list);

		return !size ? NULL : keepalived_malloc(size, file, function, line);
	}

	entry = rb_search(&alloc_list, &search, t, memcheck_ptr_cmp);

	 
	if (!entry) {
		entry = get_free_alloc_entry();

		entry->ptr = buffer;
		entry->size = size;
		entry->file = file;
		entry->func = function;
		entry->line = line;
		entry->type = !size ? FREE_NOT_ALLOC : REALLOC_NOT_ALLOC;
		entry->seq_num = seq_num++;

		if (!is_realloc)
			fprintf(log_op, "%s%-7s%9s, %9p,      at %s, %3d, %s - not found\n", format_time(),
				"free", "ERROR",
				buffer, file, line, function);
		else
			fprintf(log_op, "%s%-7s%9s, %9p, %4zu at %s, %3d, %s - not found\n", format_time(),
				"realloc", "ERROR",
				buffer, size, file, line, function);

		__set_bit(MEM_ERR_DETECT_BIT, &debug);

		list_for_each_entry_reverse(le, &free_list, l) {
			if (le->ptr == buffer &&
			    le->type == LAST_FREE) {
				fprintf
				    (log_op, "%11s-> pointer last released at [%3d:%3d], at %s, %3d, %s\n",
				     "", le->seq_num, number_alloc_list,
				     le->file, le->line,
				     le->func);

				entry->type = !size ? DOUBLE_FREE : REALLOC_DOUBLE_FREE;
				break;
			}
		};

		list_add_tail(&entry->l, &bad_list);

		return NULL;
	}

	check = entry->size + CHECK_VAL;
	if (*(unsigned long *)((char *)buffer + entry->size) != check) {
		entry2 = get_free_alloc_entry();

		*entry2 = *entry;
		entry2->type = OVERRUN;
		list_add_tail(&entry2->l, &bad_list);

		fprintf(log_op, "%s%s corrupt, buffer overrun [%3d:%3d], %9p, %4zu at %s, %3d, %s\n",
		       format_time(), !is_realloc ? "free" : "realloc",
		       entry->seq_num, number_alloc_list, buffer,
		       entry->size, file,
		       line, function);
		dump_buffer(entry->ptr,
			    entry->size + sizeof (check), log_op, TIME_STR_LEN);
		fprintf(log_op, "%*sCheck_sum\n", TIME_STR_LEN, "");
		dump_buffer((char *) &check,
			    sizeof(check), log_op, TIME_STR_LEN);

		__set_bit(MEM_ERR_DETECT_BIT, &debug);
	}

	mem_allocated -= entry->size;

	if (!size) {
		free(buffer);

		if (is_realloc) {
			fprintf(log_op, "%s%-7s[%3d:%3d], %9p, %4zu at %s, %3d, %s -> %9s, %4s at %s, %3d, %s\n",
			       format_time(), "realloc", entry->seq_num,
			       number_alloc_list, entry->ptr,
			       entry->size, entry->file,
			       entry->line, entry->func,
			       "made free", "", file, line, function);

			 
			entry2 = get_free_alloc_entry();
			*entry2 = *entry;
			entry2->type = REALLOC_ZERO_SIZE;
			entry2->file = file;
			entry2->line = line;
			entry2->func = function;
			list_add_tail(&entry2->l, &bad_list);
		}
		else
			fprintf(log_op, "%s%-7s[%3d:%3d], %9p, %4zu at %s, %3d, %s -> %9s, %4s at %s, %3d, %s\n",
			       format_time(), "free", entry->seq_num,
			       number_alloc_list, entry->ptr,
			       entry->size, entry->file,
			       entry->line, entry->func,
			       "NULL", "", file, line, function);
#ifdef _MEM_CHECK_LOG_
		if (__test_bit(MEM_CHECK_LOG_BIT, &debug))
			log_message(LOG_INFO, "%-7s[%3d:%3d], %9p, %4zu at %s, %3d, %s",
			       is_realloc ? "realloc" : "free",
			       entry->seq_num, number_alloc_list, buffer,
			       entry->size, file, line, function);
#endif

		entry->file = file;
		entry->line = line;
		entry->func = function;
		entry->type = LAST_FREE;

		rb_erase(&entry->t, &alloc_list);
		list_add_tail(&entry->l, &free_list);
		free_list_size++;

		number_alloc_list--;

		return NULL;
	}

	buffer = realloc(buffer, size + sizeof (unsigned long));
	mem_allocated += size;

	if (mem_allocated > max_mem_allocated)
		max_mem_allocated = mem_allocated;

	fprintf(log_op, "%srealloc[%3d:%3d], %9p, %4zu at %s, %3d, %s -> %9p, %4zu at %s, %3d, %s\n",
	       format_time(), entry->seq_num,
	       number_alloc_list, entry->ptr,
	       entry->size, entry->file,
	       entry->line, entry->func,
	       buffer, size, file, line, function);
#ifdef _MEM_CHECK_LOG_
	if (__test_bit(MEM_CHECK_LOG_BIT, &debug))
		log_message(LOG_INFO, "realloc[%3d:%3d], %9p, %4zu at %s, %3d, %s -> %9p, %4zu at %s, %3d, %s",
		       entry->seq_num, number_alloc_list, entry->ptr,
		       entry->size, entry->file,
		       entry->line, entry->func,
		       buffer, size, file, line, function);
#endif

	*(unsigned long *) ((char *) buffer + size) = size + CHECK_VAL;

	if (entry->ptr != buffer) {
		rb_erase(&entry->t, &alloc_list);
		entry->ptr = buffer;
		rb_insert_sort(&alloc_list, entry, t, memcheck_ptr_cmp);
	} else
		entry->ptr = buffer;
	entry->size = size;
	entry->file = file;
	entry->line = line;
	entry->func = function;

	num_reallocs++;

	return buffer;
}