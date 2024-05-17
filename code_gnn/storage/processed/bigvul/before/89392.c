 void __attribute__((no_instrument_function)) __cyg_profile_func_exit(
 		void *func_ptr, void *caller)
{
	if (trace_enabled) {
		trace_swap_gd();
		add_ftrace(func_ptr, caller, FUNCF_EXIT);
		hdr->depth--;
		trace_swap_gd();
	}
 }
