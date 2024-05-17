static int pid_delete_dentry(struct dentry * dentry)
{
	 
	return !proc_pid(dentry->d_inode)->tasks[PIDTYPE_PID].first;
}
