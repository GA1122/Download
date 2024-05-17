static int path_parent_directory(struct path *path)
{
	struct dentry *old = path->dentry;
	 
	path->dentry = dget_parent(path->dentry);
	dput(old);
	if (unlikely(!path_connected(path)))
		return -ENOENT;
	return 0;
}
