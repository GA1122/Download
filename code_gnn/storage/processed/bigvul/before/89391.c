 void __attribute__((no_instrument_function)) __cyg_profile_func_enter(
 		void *func_ptr, void *caller)
{
	if (trace_enabled) {
		int func;

		trace_swap_gd();
		add_ftrace(func_ptr, caller, FUNCF_ENTRY);
		func = func_ptr_to_num(func_ptr);
		if (func < hdr->func_count) {
			hdr->call_accum[func]++;
			hdr->call_count++;
		} else {
			hdr->untracked_count++;
		}
		hdr->depth++;
		if (hdr->depth > hdr->depth_limit)
			hdr->max_depth = hdr->depth;
		trace_swap_gd();
	}
 }
