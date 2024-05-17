 int trace_list_functions(void *buff, size_t buff_size, size_t *needed)
 {
	struct trace_output_hdr *output_hdr = NULL;
	void *end, *ptr = buff;
	size_t func;
	size_t upto;

	end = buff ? buff + buff_size : NULL;

	 
	if (ptr + sizeof(struct trace_output_hdr) < end)
		output_hdr = ptr;
	ptr += sizeof(struct trace_output_hdr);

	 
	for (func = upto = 0; func < hdr->func_count; func++) {
		size_t calls = hdr->call_accum[func];

		if (!calls)
			continue;

		if (ptr + sizeof(struct trace_output_func) < end) {
			struct trace_output_func *stats = ptr;

			stats->offset = func * FUNC_SITE_SIZE;
			stats->call_count = calls;
			upto++;
		}
		ptr += sizeof(struct trace_output_func);
	}

	 
	if (output_hdr) {
		output_hdr->rec_count = upto;
		output_hdr->type = TRACE_CHUNK_FUNCS;
	}

	 
	*needed = ptr - buff;
	if (ptr > end)
		return -ENOSPC;

 	return 0;
 }
