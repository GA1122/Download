void trace_dump_stack(int skip)
{
	unsigned long flags;

	if (tracing_disabled || tracing_selftest_running)
		return;

	local_save_flags(flags);

#ifndef CONFIG_UNWINDER_ORC
	 
	skip++;
#endif
	__ftrace_trace_stack(global_trace.trace_buffer.buffer,
			     flags, skip, preempt_count(), NULL);
}