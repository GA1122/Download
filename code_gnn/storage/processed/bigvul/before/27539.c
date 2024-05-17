static int inotify_find_inode(const char __user *dirname, struct path *path, unsigned flags)
{
	int error;

	error = user_path_at(AT_FDCWD, dirname, flags, path);
	if (error)
		return error;
	 
	error = inode_permission(path->dentry->d_inode, MAY_READ);
	if (error)
		path_put(path);
	return error;
}
