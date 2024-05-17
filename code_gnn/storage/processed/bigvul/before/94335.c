static void reparent_to_kthreadd(void)
{
	write_lock_irq(&tasklist_lock);

	ptrace_unlink(current);
	 
	current->real_parent = current->parent = kthreadd_task;
	list_move_tail(&current->sibling, &current->real_parent->children);

	 
	current->exit_signal = SIGCHLD;

	if (task_nice(current) < 0)
		set_user_nice(current, 0);
	 
	 
	 
	memcpy(current->signal->rlim, init_task.signal->rlim,
	       sizeof(current->signal->rlim));

	atomic_inc(&init_cred.usage);
	commit_creds(&init_cred);
	write_unlock_irq(&tasklist_lock);
}