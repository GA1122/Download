static void reparent_thread(struct task_struct *p, struct task_struct *father)
{
	if (p->pdeath_signal)
		 
		group_send_sig_info(p->pdeath_signal, SEND_SIG_NOINFO, p);

	list_move_tail(&p->sibling, &p->real_parent->children);

	 
	if (same_thread_group(p->real_parent, father))
		return;

	 
	if (!task_detached(p))
		p->exit_signal = SIGCHLD;

	 
	if (!ptrace_reparented(p) &&
	    p->exit_state == EXIT_ZOMBIE &&
	    !task_detached(p) && thread_group_empty(p))
		do_notify_parent(p, p->exit_signal);

	kill_orphaned_pgrp(p, father);
}
