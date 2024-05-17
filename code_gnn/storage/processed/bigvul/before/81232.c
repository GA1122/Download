static void __ftrace_trace_stack(struct ring_buffer *buffer,
				 unsigned long flags,
				 int skip, int pc, struct pt_regs *regs)
{
	struct trace_event_call *call = &event_kernel_stack;
	struct ring_buffer_event *event;
	struct stack_entry *entry;
	struct stack_trace trace;
	int use_stack;
	int size = FTRACE_STACK_ENTRIES;

	trace.nr_entries	= 0;
	trace.skip		= skip;

	 
#ifndef CONFIG_UNWINDER_ORC
	if (!regs)
		trace.skip++;
#endif

	 
	preempt_disable_notrace();

	use_stack = __this_cpu_inc_return(ftrace_stack_reserve);
	 
	barrier();
	if (use_stack == 1) {
		trace.entries		= this_cpu_ptr(ftrace_stack.calls);
		trace.max_entries	= FTRACE_STACK_MAX_ENTRIES;

		if (regs)
			save_stack_trace_regs(regs, &trace);
		else
			save_stack_trace(&trace);

		if (trace.nr_entries > size)
			size = trace.nr_entries;
	} else
		 
		use_stack = 0;

	size *= sizeof(unsigned long);

	event = __trace_buffer_lock_reserve(buffer, TRACE_STACK,
					    sizeof(*entry) + size, flags, pc);
	if (!event)
		goto out;
	entry = ring_buffer_event_data(event);

	memset(&entry->caller, 0, size);

	if (use_stack)
		memcpy(&entry->caller, trace.entries,
		       trace.nr_entries * sizeof(unsigned long));
	else {
		trace.max_entries	= FTRACE_STACK_ENTRIES;
		trace.entries		= entry->caller;
		if (regs)
			save_stack_trace_regs(regs, &trace);
		else
			save_stack_trace(&trace);
	}

	entry->size = trace.nr_entries;

	if (!call_filter_check_discard(call, entry, buffer, event))
		__buffer_unlock_commit(buffer, event);

 out:
	 
	barrier();
	__this_cpu_dec(ftrace_stack_reserve);
	preempt_enable_notrace();

}
