static struct dentry *proc_pident_lookup(struct inode *dir, 
					 struct dentry *dentry,
					 const struct pid_entry *ents,
					 unsigned int nents)
{
	int error;
	struct task_struct *task = get_proc_task(dir);
	const struct pid_entry *p, *last;

	error = -ENOENT;

	if (!task)
		goto out_no_task;

	 
	last = &ents[nents - 1];
	for (p = ents; p <= last; p++) {
		if (p->len != dentry->d_name.len)
			continue;
		if (!memcmp(dentry->d_name.name, p->name, p->len))
			break;
	}
	if (p > last)
		goto out;

	error = proc_pident_instantiate(dir, dentry, task, p);
out:
	put_task_struct(task);
out_no_task:
	return ERR_PTR(error);
}