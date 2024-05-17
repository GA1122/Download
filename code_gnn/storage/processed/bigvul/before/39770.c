static struct dentry *proc_task_instantiate(struct inode *dir,
	struct dentry *dentry, struct task_struct *task, const void *ptr)
{
	struct dentry *error = ERR_PTR(-ENOENT);
	struct inode *inode;
	inode = proc_pid_make_inode(dir->i_sb, task);

	if (!inode)
		goto out;
	inode->i_mode = S_IFDIR|S_IRUGO|S_IXUGO;
	inode->i_op = &proc_tid_base_inode_operations;
	inode->i_fop = &proc_tid_base_operations;
	inode->i_flags|=S_IMMUTABLE;

	inode->i_nlink = 2 + pid_entry_count_dirs(tid_base_stuff,
		ARRAY_SIZE(tid_base_stuff));

	dentry->d_op = &pid_dentry_operations;

	d_add(dentry, inode);
	 
	if (pid_revalidate(dentry, NULL))
		error = NULL;
out:
	return error;
}