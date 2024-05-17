static void ftrace_startup_sysctl(void)
{
	if (unlikely(ftrace_disabled))
		return;

	 
	saved_ftrace_func = NULL;
	 
	if (ftrace_start_up)
		ftrace_run_update_code(FTRACE_UPDATE_CALLS);
}
