static struct mm_struct *__check_mem_permission(struct task_struct *task)
{
	struct mm_struct *mm;

	mm = get_task_mm(task);
	if (!mm)
		return ERR_PTR(-EINVAL);

	 
	if (task == current)
		return mm;

	 
	if (task_is_stopped_or_traced(task)) {
		int match;
		rcu_read_lock();
		match = (tracehook_tracer_task(task) == current);
		rcu_read_unlock();
		if (match && ptrace_may_access(task, PTRACE_MODE_ATTACH))
			return mm;
	}

	 
	mmput(mm);
	return ERR_PTR(-EPERM);
}
