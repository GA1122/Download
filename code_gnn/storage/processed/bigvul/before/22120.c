void mm_update_next_owner(struct mm_struct *mm)
{
	struct task_struct *c, *g, *p = current;

retry:
	if (!mm_need_new_owner(mm, p))
		return;

	read_lock(&tasklist_lock);
	 
	list_for_each_entry(c, &p->children, sibling) {
		if (c->mm == mm)
			goto assign_new_owner;
	}

	 
	list_for_each_entry(c, &p->parent->children, sibling) {
		if (c->mm == mm)
			goto assign_new_owner;
	}

	 
	do_each_thread(g, c) {
		if (c->mm == mm)
			goto assign_new_owner;
	} while_each_thread(g, c);

	read_unlock(&tasklist_lock);
	 
	down_write(&mm->mmap_sem);
	cgroup_mm_owner_callbacks(mm->owner, NULL);
	mm->owner = NULL;
	up_write(&mm->mmap_sem);
	return;

assign_new_owner:
	BUG_ON(c == p);
	get_task_struct(c);
	read_unlock(&tasklist_lock);
	down_write(&mm->mmap_sem);
	 
	task_lock(c);
	if (c->mm != mm) {
		task_unlock(c);
		up_write(&mm->mmap_sem);
		put_task_struct(c);
		goto retry;
	}
	cgroup_mm_owner_callbacks(mm->owner, c);
	mm->owner = c;
	task_unlock(c);
	up_write(&mm->mmap_sem);
	put_task_struct(c);
}
