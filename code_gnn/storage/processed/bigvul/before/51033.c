mountpoint_last(struct nameidata *nd, struct path *path)
{
	int error = 0;
	struct dentry *dentry;
	struct dentry *dir = nd->path.dentry;

	 
	if (nd->flags & LOOKUP_RCU) {
		if (unlazy_walk(nd, NULL, 0))
			return -ECHILD;
	}

	nd->flags &= ~LOOKUP_PARENT;

	if (unlikely(nd->last_type != LAST_NORM)) {
		error = handle_dots(nd, nd->last_type);
		if (error)
			return error;
		dentry = dget(nd->path.dentry);
	} else {
		dentry = d_lookup(dir, &nd->last);
		if (!dentry) {
			 
			dentry = lookup_slow(&nd->last, dir,
					     nd->flags | LOOKUP_NO_REVAL);
			if (IS_ERR(dentry))
				return PTR_ERR(dentry);
		}
	}
	if (d_is_negative(dentry)) {
		dput(dentry);
		return -ENOENT;
	}
	if (nd->depth)
		put_link(nd);
	path->dentry = dentry;
	path->mnt = nd->path.mnt;
	error = should_follow_link(nd, path, nd->flags & LOOKUP_FOLLOW,
				   d_backing_inode(dentry), 0);
	if (unlikely(error))
		return error;
	mntget(path->mnt);
	follow_mount(path);
	return 0;
}
