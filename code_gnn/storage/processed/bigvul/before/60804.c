static void reparent_leader(struct task_struct *father, struct task_struct *p,
				struct list_head *dead)
{
	if (unlikely(p->exit_state == EXIT_DEAD))
		return;

	 
	p->exit_signal = SIGCHLD;

	 
	if (!p->ptrace &&
	    p->exit_state == EXIT_ZOMBIE && thread_group_empty(p)) {
		if (do_notify_parent(p, p->exit_signal)) {
			p->exit_state = EXIT_DEAD;
			list_add(&p->ptrace_entry, dead);
		}
	}

	kill_orphaned_pgrp(p, father);
}