static void update_ftrace_function(void)
{
	ftrace_func_t func;

	update_global_ops();

	 
	if (ftrace_ops_list == &ftrace_list_end ||
	    (ftrace_ops_list->next == &ftrace_list_end &&
	     !(ftrace_ops_list->flags & FTRACE_OPS_FL_DYNAMIC) &&
	     (ftrace_ops_list->flags & FTRACE_OPS_FL_RECURSION_SAFE) &&
	     !FTRACE_FORCE_LIST_FUNC)) {
		 
		if (ftrace_ops_list == &global_ops)
			function_trace_op = ftrace_global_list;
		else
			function_trace_op = ftrace_ops_list;
		func = ftrace_ops_list->func;
	} else {
		 
		function_trace_op = &ftrace_list_end;
		func = ftrace_ops_list_func;
	}

	ftrace_trace_function = func;
}
