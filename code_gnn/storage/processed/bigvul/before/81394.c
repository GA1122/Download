void trace_filter_add_remove_task(struct trace_pid_list *pid_list,
				  struct task_struct *self,
				  struct task_struct *task)
{
	if (!pid_list)
		return;

	 
	if (self) {
		if (!trace_find_filtered_pid(pid_list, self->pid))
			return;
	}

	 
	if (task->pid >= pid_list->pid_max)
		return;

	 
	if (self)
		set_bit(task->pid, pid_list->pids);
	else
		clear_bit(task->pid, pid_list->pids);
}
