static void ftrace_update_pid_func(void)
{
	 
	if (ftrace_trace_function == ftrace_stub)
		return;

	update_ftrace_function();
}
