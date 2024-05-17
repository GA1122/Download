static int ptrace_check_attach(struct task_struct *child, bool ignore_state)
{
	int ret = -ESRCH;

	 
	read_lock(&tasklist_lock);
	if (child->ptrace && child->parent == current) {
		WARN_ON(child->state == __TASK_TRACED);
		 
		if (ignore_state || ptrace_freeze_traced(child))
			ret = 0;
	}
	read_unlock(&tasklist_lock);

	if (!ret && !ignore_state) {
		if (!wait_task_inactive(child, __TASK_TRACED)) {
			 
			WARN_ON(child->state == __TASK_TRACED);
			ret = -ESRCH;
		}
	}

	return ret;
}
