static void __attribute__((no_instrument_function)) add_textbase(void)
{
	if (hdr->ftrace_count < hdr->ftrace_size) {
		struct trace_call *rec = &hdr->ftrace[hdr->ftrace_count];

		rec->func = CONFIG_SYS_TEXT_BASE;
		rec->caller = 0;
		rec->flags = FUNCF_TEXTBASE;
	}
	hdr->ftrace_count++;
 }
