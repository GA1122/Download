static int check_mem_permission(struct task_struct *task)
{
	 
	if (task == current)
		return 0;

	 
	if (task_is_stopped_or_traced(task)) {
		int match;
		rcu_read_lock();
		match = (tracehook_tracer_task(task) == current);
		rcu_read_unlock();
		if (match && ptrace_may_access(task, PTRACE_MODE_ATTACH))
			return 0;
	}

	 
	return -EPERM;
}
