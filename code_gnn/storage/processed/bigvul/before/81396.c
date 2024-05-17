trace_find_filtered_pid(struct trace_pid_list *filtered_pids, pid_t search_pid)
{
	 
	if (search_pid >= filtered_pids->pid_max)
		return false;

	return test_bit(search_pid, filtered_pids->pids);
}
