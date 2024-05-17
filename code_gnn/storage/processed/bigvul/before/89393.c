static void __attribute__((no_instrument_function)) add_ftrace(void *func_ptr,
				void *caller, ulong flags)
{
	if (hdr->depth > hdr->depth_limit) {
		hdr->ftrace_too_deep_count++;
		return;
	}
	if (hdr->ftrace_count < hdr->ftrace_size) {
		struct trace_call *rec = &hdr->ftrace[hdr->ftrace_count];

		rec->func = func_ptr_to_num(func_ptr);
		rec->caller = func_ptr_to_num(caller);
		rec->flags = flags | (timer_get_us() & FUNCF_TIMESTAMP_MASK);
	}
	hdr->ftrace_count++;
}
