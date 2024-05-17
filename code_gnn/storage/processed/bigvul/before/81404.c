trace_ignore_this_task(struct trace_pid_list *filtered_pids, struct task_struct *task)
{
	 
	if (!filtered_pids)
		return false;

	return !trace_find_filtered_pid(filtered_pids, task->pid);
}
