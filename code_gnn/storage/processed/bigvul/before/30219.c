static void ftrace_run_update_code(int command)
{
	int ret;

	ret = ftrace_arch_code_modify_prepare();
	FTRACE_WARN_ON(ret);
	if (ret)
		return;
	 
	function_trace_stop++;

	 
	arch_ftrace_update_code(command);

	function_trace_stop--;

	ret = ftrace_arch_code_modify_post_process();
	FTRACE_WARN_ON(ret);
}
