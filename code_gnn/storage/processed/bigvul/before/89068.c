static int ptrace_detach(struct task_struct *child, unsigned int data)
{
	if (!valid_signal(data))
		return -EIO;

	 
	ptrace_disable(child);

	write_lock_irq(&tasklist_lock);
	 
	WARN_ON(!child->ptrace || child->exit_state);
	 
	child->exit_code = data;
	__ptrace_detach(current, child);
	write_unlock_irq(&tasklist_lock);

	proc_ptrace_connector(child, PTRACE_DETACH);

	return 0;
}
