static int ovl_remove_and_whiteout(struct dentry *dentry, bool is_dir)
{
	struct dentry *workdir = ovl_workdir(dentry);
	struct inode *wdir = workdir->d_inode;
	struct dentry *upperdir = ovl_dentry_upper(dentry->d_parent);
	struct inode *udir = upperdir->d_inode;
	struct dentry *whiteout;
	struct dentry *upper;
	struct dentry *opaquedir = NULL;
	int err;

	if (WARN_ON(!workdir))
		return -EROFS;

	if (is_dir) {
		if (OVL_TYPE_MERGE_OR_LOWER(ovl_path_type(dentry))) {
			opaquedir = ovl_check_empty_and_clear(dentry);
			err = PTR_ERR(opaquedir);
			if (IS_ERR(opaquedir))
				goto out;
		} else {
			LIST_HEAD(list);

			 
			err = ovl_check_empty_dir(dentry, &list);
			ovl_cache_free(&list);
			if (err)
				goto out;
		}
	}

	err = ovl_lock_rename_workdir(workdir, upperdir);
	if (err)
		goto out_dput;

	whiteout = ovl_whiteout(workdir, dentry);
	err = PTR_ERR(whiteout);
	if (IS_ERR(whiteout))
		goto out_unlock;

	upper = ovl_dentry_upper(dentry);
	if (!upper) {
		upper = lookup_one_len(dentry->d_name.name, upperdir,
				       dentry->d_name.len);
		err = PTR_ERR(upper);
		if (IS_ERR(upper))
			goto kill_whiteout;

		err = ovl_do_rename(wdir, whiteout, udir, upper, 0);
		dput(upper);
		if (err)
			goto kill_whiteout;
	} else {
		int flags = 0;

		if (opaquedir)
			upper = opaquedir;
		err = -ESTALE;
		if (upper->d_parent != upperdir)
			goto kill_whiteout;

		if (is_dir)
			flags |= RENAME_EXCHANGE;

		err = ovl_do_rename(wdir, whiteout, udir, upper, flags);
		if (err)
			goto kill_whiteout;

		if (is_dir)
			ovl_cleanup(wdir, upper);
	}
	ovl_dentry_version_inc(dentry->d_parent);
out_d_drop:
	d_drop(dentry);
	dput(whiteout);
out_unlock:
	unlock_rename(workdir, upperdir);
out_dput:
	dput(opaquedir);
out:
	return err;

kill_whiteout:
	ovl_cleanup(wdir, whiteout);
	goto out_d_drop;
}