void mm_update_next_owner(struct mm_struct *mm)
{
	struct task_struct *c, *g, *p = current;

retry:
	 
	if (mm->owner != p)
		return;
	 
	if (atomic_read(&mm->mm_users) <= 1) {
		mm->owner = NULL;
		return;
	}

	read_lock(&tasklist_lock);
	 
	list_for_each_entry(c, &p->children, sibling) {
		if (c->mm == mm)
			goto assign_new_owner;
	}

	 
	list_for_each_entry(c, &p->real_parent->children, sibling) {
		if (c->mm == mm)
			goto assign_new_owner;
	}

	 
	for_each_process(g) {
		if (g->flags & PF_KTHREAD)
			continue;
		for_each_thread(g, c) {
			if (c->mm == mm)
				goto assign_new_owner;
			if (c->mm)
				break;
		}
	}
	read_unlock(&tasklist_lock);
	 
	mm->owner = NULL;
	return;

assign_new_owner:
	BUG_ON(c == p);
	get_task_struct(c);
	 
	task_lock(c);
	 
	read_unlock(&tasklist_lock);
	if (c->mm != mm) {
		task_unlock(c);
		put_task_struct(c);
		goto retry;
	}
	mm->owner = c;
	task_unlock(c);
	put_task_struct(c);
}
