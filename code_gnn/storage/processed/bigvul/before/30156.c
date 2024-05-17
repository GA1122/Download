ftrace_graph_probe_sched_switch(void *ignore,
			struct task_struct *prev, struct task_struct *next)
{
	unsigned long long timestamp;
	int index;

	 
	if (trace_flags & TRACE_ITER_SLEEP_TIME)
		return;

	timestamp = trace_clock_local();

	prev->ftrace_timestamp = timestamp;

	 
	if (!next->ftrace_timestamp)
		return;

	 
	timestamp -= next->ftrace_timestamp;

	for (index = next->curr_ret_stack; index >= 0; index--)
		next->ret_stack[index].calltime += timestamp;
}
