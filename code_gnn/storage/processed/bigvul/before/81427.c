void *trace_pid_next(struct trace_pid_list *pid_list, void *v, loff_t *pos)
{
	unsigned long pid = (unsigned long)v;

	(*pos)++;

	 
	pid = find_next_bit(pid_list->pids, pid_list->pid_max, pid);

	 
	if (pid < pid_list->pid_max)
		return (void *)(pid + 1);

	return NULL;
}
