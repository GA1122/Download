static int do_last(struct nameidata *nd, struct path *path,
		   struct file *file, const struct open_flags *op,
		   int *opened, struct filename *name)
{
	struct dentry *dir = nd->path.dentry;
	int open_flag = op->open_flag;
	bool will_truncate = (open_flag & O_TRUNC) != 0;
	bool got_write = false;
	int acc_mode = op->acc_mode;
	struct inode *inode;
	bool symlink_ok = false;
	struct path save_parent = { .dentry = NULL, .mnt = NULL };
	bool retried = false;
	int error;

	nd->flags &= ~LOOKUP_PARENT;
	nd->flags |= op->intent;

	if (nd->last_type != LAST_NORM) {
		error = handle_dots(nd, nd->last_type);
		if (error)
			return error;
		goto finish_open;
	}

	if (!(open_flag & O_CREAT)) {
		if (nd->last.name[nd->last.len])
			nd->flags |= LOOKUP_FOLLOW | LOOKUP_DIRECTORY;
		if (open_flag & O_PATH && !(nd->flags & LOOKUP_FOLLOW))
			symlink_ok = true;
		 
		error = lookup_fast(nd, path, &inode);
		if (likely(!error))
			goto finish_lookup;

		if (error < 0)
			goto out;

		BUG_ON(nd->inode != dir->d_inode);
	} else {
		 
		 
		error = complete_walk(nd);
		if (error)
			return error;

		audit_inode(name, dir, LOOKUP_PARENT);
		error = -EISDIR;
		 
		if (nd->last.name[nd->last.len])
			goto out;
	}

retry_lookup:
	if (op->open_flag & (O_CREAT | O_TRUNC | O_WRONLY | O_RDWR)) {
		error = mnt_want_write(nd->path.mnt);
		if (!error)
			got_write = true;
		 
	}
	mutex_lock(&dir->d_inode->i_mutex);
	error = lookup_open(nd, path, file, op, got_write, opened);
	mutex_unlock(&dir->d_inode->i_mutex);

	if (error <= 0) {
		if (error)
			goto out;

		if ((*opened & FILE_CREATED) ||
		    !S_ISREG(file_inode(file)->i_mode))
			will_truncate = false;

		audit_inode(name, file->f_path.dentry, 0);
		goto opened;
	}

	if (*opened & FILE_CREATED) {
		 
		open_flag &= ~O_TRUNC;
		will_truncate = false;
		acc_mode = MAY_OPEN;
		path_to_nameidata(path, nd);
		goto finish_open_created;
	}

	 
	if (d_is_positive(path->dentry))
		audit_inode(name, path->dentry, 0);

	 
	if (got_write) {
		mnt_drop_write(nd->path.mnt);
		got_write = false;
	}

	error = -EEXIST;
	if ((open_flag & (O_EXCL | O_CREAT)) == (O_EXCL | O_CREAT))
		goto exit_dput;

	error = follow_managed(path, nd->flags);
	if (error < 0)
		goto exit_dput;

	if (error)
		nd->flags |= LOOKUP_JUMPED;

	BUG_ON(nd->flags & LOOKUP_RCU);
	inode = path->dentry->d_inode;
	error = -ENOENT;
	if (d_is_negative(path->dentry)) {
		path_to_nameidata(path, nd);
		goto out;
	}
finish_lookup:
	 
	if (should_follow_link(path->dentry, !symlink_ok)) {
		if (nd->flags & LOOKUP_RCU) {
			if (unlikely(nd->path.mnt != path->mnt ||
				     unlazy_walk(nd, path->dentry))) {
				error = -ECHILD;
				goto out;
			}
		}
		BUG_ON(inode != path->dentry->d_inode);
		return 1;
	}

	if ((nd->flags & LOOKUP_RCU) || nd->path.mnt != path->mnt) {
		path_to_nameidata(path, nd);
	} else {
		save_parent.dentry = nd->path.dentry;
		save_parent.mnt = mntget(path->mnt);
		nd->path.dentry = path->dentry;

	}
	nd->inode = inode;
	 
finish_open:
	error = complete_walk(nd);
	if (error) {
		path_put(&save_parent);
		return error;
	}
	audit_inode(name, nd->path.dentry, 0);
	error = -EISDIR;
	if ((open_flag & O_CREAT) && d_is_dir(nd->path.dentry))
		goto out;
	error = -ENOTDIR;
	if ((nd->flags & LOOKUP_DIRECTORY) && !d_can_lookup(nd->path.dentry))
		goto out;
	if (!d_is_reg(nd->path.dentry))
		will_truncate = false;

	if (will_truncate) {
		error = mnt_want_write(nd->path.mnt);
		if (error)
			goto out;
		got_write = true;
	}
finish_open_created:
	error = may_open(&nd->path, acc_mode, open_flag);
	if (error)
		goto out;

	BUG_ON(*opened & FILE_OPENED);  
	error = vfs_open(&nd->path, file, current_cred());
	if (!error) {
		*opened |= FILE_OPENED;
	} else {
		if (error == -EOPENSTALE)
			goto stale_open;
		goto out;
	}
opened:
	error = open_check_o_direct(file);
	if (error)
		goto exit_fput;
	error = ima_file_check(file, op->acc_mode, *opened);
	if (error)
		goto exit_fput;

	if (will_truncate) {
		error = handle_truncate(file);
		if (error)
			goto exit_fput;
	}
out:
	if (got_write)
		mnt_drop_write(nd->path.mnt);
	path_put(&save_parent);
	terminate_walk(nd);
	return error;

exit_dput:
	path_put_conditional(path, nd);
	goto out;
exit_fput:
	fput(file);
	goto out;

stale_open:
	 
	if (!save_parent.dentry || retried)
		goto out;

	BUG_ON(save_parent.dentry != dir);
	path_put(&nd->path);
	nd->path = save_parent;
	nd->inode = dir->d_inode;
	save_parent.mnt = NULL;
	save_parent.dentry = NULL;
	if (got_write) {
		mnt_drop_write(nd->path.mnt);
		got_write = false;
	}
	retried = true;
	goto retry_lookup;
}