 void trace_print_stats(void)
 {
 	ulong count;

#ifndef FTRACE
	puts("Warning: make U-Boot with FTRACE to enable function instrumenting.\n");
	puts("You will likely get zeroed data here\n");
#endif
	if (!trace_inited) {
		printf("Trace is disabled\n");
		return;
	}
	print_grouped_ull(hdr->func_count, 10);
	puts(" function sites\n");
	print_grouped_ull(hdr->call_count, 10);
	puts(" function calls\n");
	print_grouped_ull(hdr->untracked_count, 10);
	puts(" untracked function calls\n");
	count = min(hdr->ftrace_count, hdr->ftrace_size);
	print_grouped_ull(count, 10);
	puts(" traced function calls");
	if (hdr->ftrace_count > hdr->ftrace_size) {
		printf(" (%lu dropped due to overflow)",
		       hdr->ftrace_count - hdr->ftrace_size);
	}
	puts("\n");
	printf("%15d maximum observed call depth\n", hdr->max_depth);
	printf("%15d call depth limit\n", hdr->depth_limit);
	print_grouped_ull(hdr->ftrace_too_deep_count, 10);
	puts(" calls not traced due to depth\n");
}
