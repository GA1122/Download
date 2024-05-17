static void reparent_thread(struct task_struct *father, struct task_struct *p,
				struct list_head *dead)
{
	if (p->pdeath_signal)
		group_send_sig_info(p->pdeath_signal, SEND_SIG_NOINFO, p);

	list_move_tail(&p->sibling, &p->real_parent->children);

	if (task_detached(p))
		return;
	 
	if (same_thread_group(p->real_parent, father))
		return;

	 
	p->exit_signal = SIGCHLD;

	 
	if (!task_ptrace(p) &&
	    p->exit_state == EXIT_ZOMBIE && thread_group_empty(p)) {
		do_notify_parent(p, p->exit_signal);
		if (task_detached(p)) {
			p->exit_state = EXIT_DEAD;
			list_move_tail(&p->sibling, dead);
		}
	}

	kill_orphaned_pgrp(p, father);
}
