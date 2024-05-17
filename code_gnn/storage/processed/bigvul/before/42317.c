static struct dentry *filename_create(int dfd, struct filename *name,
				struct path *path, unsigned int lookup_flags)
{
	struct dentry *dentry = ERR_PTR(-EEXIST);
	struct nameidata nd;
	int err2;
	int error;
	bool is_dir = (lookup_flags & LOOKUP_DIRECTORY);

	 
	lookup_flags &= LOOKUP_REVAL;

	error = filename_lookup(dfd, name, LOOKUP_PARENT|lookup_flags, &nd);
	if (error)
		return ERR_PTR(error);

	 
	if (nd.last_type != LAST_NORM)
		goto out;
	nd.flags &= ~LOOKUP_PARENT;
	nd.flags |= LOOKUP_CREATE | LOOKUP_EXCL;

	 
	err2 = mnt_want_write(nd.path.mnt);
	 
	mutex_lock_nested(&nd.path.dentry->d_inode->i_mutex, I_MUTEX_PARENT);
	dentry = lookup_hash(&nd);
	if (IS_ERR(dentry))
		goto unlock;

	error = -EEXIST;
	if (d_is_positive(dentry))
		goto fail;

	 
	if (unlikely(!is_dir && nd.last.name[nd.last.len])) {
		error = -ENOENT;
		goto fail;
	}
	if (unlikely(err2)) {
		error = err2;
		goto fail;
	}
	*path = nd.path;
	return dentry;
fail:
	dput(dentry);
	dentry = ERR_PTR(error);
unlock:
	mutex_unlock(&nd.path.dentry->d_inode->i_mutex);
	if (!err2)
		mnt_drop_write(nd.path.mnt);
out:
	path_put(&nd.path);
	return dentry;
}
