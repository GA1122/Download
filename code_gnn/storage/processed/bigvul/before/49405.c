int pid_delete_dentry(const struct dentry *dentry)
{
	 
	return proc_inode_is_dead(d_inode(dentry));
}
