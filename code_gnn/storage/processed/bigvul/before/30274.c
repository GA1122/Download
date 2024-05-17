static void set_ftrace_pid_function(ftrace_func_t func)
{
	 
	if (func != ftrace_pid_func)
		ftrace_pid_function = func;
}
