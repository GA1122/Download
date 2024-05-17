static int do_lookup(struct nameidata *nd, struct qstr *name,
		     struct path *path)
{
	struct vfsmount *mnt = nd->path.mnt;
	struct dentry *dentry, *parent;
	struct inode *dir;
	 
	if (nd->path.dentry->d_op && nd->path.dentry->d_op->d_hash) {
		int err = nd->path.dentry->d_op->d_hash(nd->path.dentry, name);
		if (err < 0)
			return err;
	}

	dentry = __d_lookup(nd->path.dentry, name);
	if (!dentry)
		goto need_lookup;
	if (dentry->d_op && dentry->d_op->d_revalidate)
		goto need_revalidate;
done:
	path->mnt = mnt;
	path->dentry = dentry;
	__follow_mount(path);
	return 0;

need_lookup:
	parent = nd->path.dentry;
	dir = parent->d_inode;

	mutex_lock(&dir->i_mutex);
	 
	dentry = d_lookup(parent, name);
	if (!dentry) {
		struct dentry *new;

		 
		dentry = ERR_PTR(-ENOENT);
		if (IS_DEADDIR(dir))
			goto out_unlock;

		new = d_alloc(parent, name);
		dentry = ERR_PTR(-ENOMEM);
		if (new) {
			dentry = dir->i_op->lookup(dir, new, nd);
			if (dentry)
				dput(new);
			else
				dentry = new;
		}
out_unlock:
		mutex_unlock(&dir->i_mutex);
		if (IS_ERR(dentry))
			goto fail;
		goto done;
	}

	 
	mutex_unlock(&dir->i_mutex);
	if (dentry->d_op && dentry->d_op->d_revalidate) {
		dentry = do_revalidate(dentry, nd);
		if (!dentry)
			dentry = ERR_PTR(-ENOENT);
	}
	if (IS_ERR(dentry))
		goto fail;
	goto done;

need_revalidate:
	dentry = do_revalidate(dentry, nd);
	if (!dentry)
		goto need_lookup;
	if (IS_ERR(dentry))
		goto fail;
	goto done;

fail:
	return PTR_ERR(dentry);
}
