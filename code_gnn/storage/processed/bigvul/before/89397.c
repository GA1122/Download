 int __attribute__((no_instrument_function)) trace_init(void *buff,
 		size_t buff_size)
{
	ulong func_count = gd->mon_len / FUNC_SITE_SIZE;
	size_t needed;
	int was_disabled = !trace_enabled;

	trace_save_gd();

	if (!was_disabled) {
#ifdef CONFIG_TRACE_EARLY
		char *end;
		ulong used;

		 
		trace_enabled = 0;
		hdr = map_sysmem(CONFIG_TRACE_EARLY_ADDR,
				 CONFIG_TRACE_EARLY_SIZE);
		end = (char *)&hdr->ftrace[min(hdr->ftrace_count,
					       hdr->ftrace_size)];
		used = end - (char *)hdr;
		printf("trace: copying %08lx bytes of early data from %x to %08lx\n",
		       used, CONFIG_TRACE_EARLY_ADDR,
		       (ulong)map_to_sysmem(buff));
		memcpy(buff, hdr, used);
#else
		puts("trace: already enabled\n");
		return -EALREADY;
#endif
	}
	hdr = (struct trace_hdr *)buff;
	needed = sizeof(*hdr) + func_count * sizeof(uintptr_t);
	if (needed > buff_size) {
		printf("trace: buffer size %zd bytes: at least %zd needed\n",
		       buff_size, needed);
		return -ENOSPC;
	}

	if (was_disabled)
		memset(hdr, '\0', needed);
	hdr->func_count = func_count;
	hdr->call_accum = (uintptr_t *)(hdr + 1);

	 
	hdr->ftrace = (struct trace_call *)(buff + needed);
	hdr->ftrace_size = (buff_size - needed) / sizeof(*hdr->ftrace);
	add_textbase();

	puts("trace: enabled\n");
	hdr->depth_limit = CONFIG_TRACE_CALL_DEPTH_LIMIT;
	trace_enabled = 1;
	trace_inited = 1;

	return 0;
 }
