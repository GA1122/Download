void __ptrace_unlink(struct task_struct *child)
{
	BUG_ON(!child->ptrace);

	child->ptrace = 0;
	child->parent = child->real_parent;
	list_del_init(&child->ptrace_entry);

	spin_lock(&child->sighand->siglock);

	 
	task_clear_jobctl_pending(child, JOBCTL_TRAP_MASK);
	task_clear_jobctl_trapping(child);

	 
	if (!(child->flags & PF_EXITING) &&
	    (child->signal->flags & SIGNAL_STOP_STOPPED ||
	     child->signal->group_stop_count)) {
		child->jobctl |= JOBCTL_STOP_PENDING;

		 
		if (!(child->jobctl & JOBCTL_STOP_SIGMASK))
			child->jobctl |= SIGSTOP;
	}

	 
	if (child->jobctl & JOBCTL_STOP_PENDING || task_is_traced(child))
		ptrace_signal_wake_up(child, true);

 	spin_unlock(&child->sighand->siglock);
 }
