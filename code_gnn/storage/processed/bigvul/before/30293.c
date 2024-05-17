static void update_global_ops(void)
{
	ftrace_func_t func;

	 
	if (ftrace_global_list == &ftrace_list_end ||
	    ftrace_global_list->next == &ftrace_list_end) {
		func = ftrace_global_list->func;
		 
		if (ftrace_global_list->flags & FTRACE_OPS_FL_RECURSION_SAFE)
			global_ops.flags |= FTRACE_OPS_FL_RECURSION_SAFE;
		else
			global_ops.flags &= ~FTRACE_OPS_FL_RECURSION_SAFE;
	} else {
		func = ftrace_global_list_func;
		 
		global_ops.flags |= FTRACE_OPS_FL_RECURSION_SAFE;
	}


	 
	if (!list_empty(&ftrace_pids)) {
		set_ftrace_pid_function(func);
		func = ftrace_pid_func;
	}

	global_ops.func = func;
}
