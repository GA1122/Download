perf_ftrace_function_call(unsigned long ip, unsigned long parent_ip,
			  struct ftrace_ops *ops, struct pt_regs *pt_regs)
{
	struct ftrace_entry *entry;
	struct hlist_head *head;
	struct pt_regs regs;
	int rctx;

	head = this_cpu_ptr(event_function.perf_events);
	if (hlist_empty(head))
		return;

#define ENTRY_SIZE (ALIGN(sizeof(struct ftrace_entry) + sizeof(u32), \
		    sizeof(u64)) - sizeof(u32))

	BUILD_BUG_ON(ENTRY_SIZE > PERF_MAX_TRACE_SIZE);

	perf_fetch_caller_regs(&regs);

	entry = perf_trace_buf_prepare(ENTRY_SIZE, TRACE_FN, NULL, &rctx);
	if (!entry)
		return;

	entry->ip = ip;
	entry->parent_ip = parent_ip;
	perf_trace_buf_submit(entry, ENTRY_SIZE, rctx, 0,
			      1, &regs, head, NULL);

#undef ENTRY_SIZE
}