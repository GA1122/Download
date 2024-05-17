keepalived_alloc_log(bool final)
{
	unsigned int overrun = 0, badptr = 0, zero_size = 0;
	size_t sum = 0;
	MEMCHECK *entry;

	if (final) {
		 
		if (skip_mem_check_final)
			return;

		fprintf(log_op, "\n---[ Keepalived memory dump for (%s) ]---\n\n", terminate_banner);
	}
	else
		fprintf(log_op, "\n---[ Keepalived memory dump for (%s) at %s ]---\n\n", terminate_banner, format_time());

	 
	if (!RB_EMPTY_ROOT(&alloc_list)) {
		fprintf(log_op, "Entries %s\n\n", final ? "not released" : "currently allocated");
		rb_for_each_entry(entry, &alloc_list, t) {
			sum += entry->size;
			fprintf(log_op, "%9p [%3d:%3d], %4zu at %s, %3d, %s",
			       entry->ptr, entry->seq_num, number_alloc_list,
			       entry->size, entry->file, entry->line, entry->func);
			if (entry->type != ALLOCATED)
				fprintf(log_op, " type = %d", entry->type);
			fprintf(log_op, "\n");
		}
	}

	if (!list_empty(&bad_list)) {
		if (!RB_EMPTY_ROOT(&alloc_list))
			fprintf(log_op, "\n");
		fprintf(log_op, "Bad entry list\n\n");

		list_for_each_entry(entry, &bad_list, l) {
			switch (entry->type) {
			case FREE_NULL:
				badptr++;
				fprintf(log_op, "%9s %9s, %4s at %s, %3d, %s - null pointer to free\n",
				       "NULL", "", "", entry->file, entry->line, entry->func);
				break;
			case REALLOC_NULL:
				badptr++;
				fprintf(log_op, "%9s %9s, %4zu at %s, %3d, %s - null pointer to realloc (converted to malloc)\n",
				     "NULL", "", entry->size, entry->file, entry->line, entry->func);
				break;
			case FREE_NOT_ALLOC:
				badptr++;
				fprintf(log_op, "%9p %9s, %4s at %s, %3d, %s - pointer not found for free\n",
				     entry->ptr, "", "", entry->file, entry->line, entry->func);
				break;
			case REALLOC_NOT_ALLOC:
				badptr++;
				fprintf(log_op, "%9p %9s, %4zu at %s, %3d, %s - pointer not found for realloc\n",
				     entry->ptr, "", entry->size, entry->file, entry->line, entry->func);
				break;
			case DOUBLE_FREE:
				badptr++;
				fprintf(log_op, "%9p %9s, %4s at %s, %3d, %s - double free of pointer\n",
				     entry->ptr, "", "", entry->file, entry->line, entry->func);
				break;
			case REALLOC_DOUBLE_FREE:
				badptr++;
				fprintf(log_op, "%9p %9s, %4zu at %s, %3d, %s - realloc 0 size already freed\n",
				     entry->ptr, "", entry->size, entry->file, entry->line, entry->func);
				break;
			case OVERRUN:
				overrun++;
				fprintf(log_op, "%9p [%3d:%3d], %4zu at %s, %3d, %s - buffer overrun\n",
				       entry->ptr, entry->seq_num, number_alloc_list,
				       entry->size, entry->file, entry->line, entry->func);
				break;
			case MALLOC_ZERO_SIZE:
				zero_size++;
				fprintf(log_op, "%9p [%3d:%3d], %4zu at %s, %3d, %s - malloc zero size\n",
				       entry->ptr, entry->seq_num, number_alloc_list,
				       entry->size, entry->file, entry->line, entry->func);
				break;
			case REALLOC_ZERO_SIZE:
				zero_size++;
				fprintf(log_op, "%9p [%3d:%3d], %4zu at %s, %3d, %s - realloc zero size (handled as free)\n",
				       entry->ptr, entry->seq_num, number_alloc_list,
				       entry->size, entry->file, entry->line, entry->func);
				break;
			case ALLOCATED:	 
			case FREE_SLOT:
			case LAST_FREE:
				break;
			}
		}
	}

	fprintf(log_op, "\n\n---[ Keepalived memory dump summary for (%s) ]---\n", terminate_banner);
	fprintf(log_op, "Total number of bytes %s...: %zu\n", final ? "not freed" : "allocated", sum);
	fprintf(log_op, "Number of entries %s.......: %d\n", final ? "not freed" : "allocated", number_alloc_list);
	fprintf(log_op, "Maximum allocated entries.........: %d\n", max_alloc_list);
	fprintf(log_op, "Maximum memory allocated..........: %zu\n", max_mem_allocated);
	fprintf(log_op, "Number of mallocs.................: %d\n", num_mallocs);
	fprintf(log_op, "Number of reallocs................: %d\n", num_reallocs);
	fprintf(log_op, "Number of bad entries.............: %d\n", badptr);
	fprintf(log_op, "Number of buffer overrun..........: %d\n", overrun);
	fprintf(log_op, "Number of 0 size allocations......: %d\n\n", zero_size);
	if (sum != mem_allocated)
		fprintf(log_op, "ERROR - sum of allocated %zu != mem_allocated %zu\n", sum, mem_allocated);

	if (final) {
		if (sum || number_alloc_list || badptr || overrun)
			fprintf(log_op, "=> Program seems to have some memory problem !!!\n\n");
		else
			fprintf(log_op, "=> Program seems to be memory allocation safe...\n\n");
	}
}