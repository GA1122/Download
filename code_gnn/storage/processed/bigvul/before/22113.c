static void forget_original_parent(struct task_struct *father)
{
	struct task_struct *p, *n, *reaper;
	LIST_HEAD(ptrace_dead);

	write_lock_irq(&tasklist_lock);
	reaper = find_new_reaper(father);
	 
	ptrace_exit(father, &ptrace_dead);

	list_for_each_entry_safe(p, n, &father->children, sibling) {
		p->real_parent = reaper;
		if (p->parent == father) {
			BUG_ON(p->ptrace);
			p->parent = p->real_parent;
		}
		reparent_thread(p, father);
	}

	write_unlock_irq(&tasklist_lock);
	BUG_ON(!list_empty(&father->children));

	ptrace_exit_finish(father, &ptrace_dead);
}