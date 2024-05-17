static void ptrace_exit(struct task_struct *parent, struct list_head *dead)
{
	struct task_struct *p, *n;
	int ign = -1;

	list_for_each_entry_safe(p, n, &parent->ptraced, ptrace_entry) {
		__ptrace_unlink(p);

		if (p->exit_state != EXIT_ZOMBIE)
			continue;

		 
		if (!task_detached(p) && thread_group_empty(p)) {
			if (!same_thread_group(p->real_parent, parent))
				do_notify_parent(p, p->exit_signal);
			else {
				if (ign < 0)
					ign = ignoring_children(parent);
				if (ign)
					p->exit_signal = -1;
			}
		}

		if (task_detached(p)) {
			 
			p->exit_state = EXIT_DEAD;
			list_add(&p->ptrace_entry, dead);
		}
	}
}
