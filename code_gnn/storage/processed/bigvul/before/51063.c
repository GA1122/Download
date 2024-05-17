static int ovl_lock_rename_workdir(struct dentry *workdir,
				   struct dentry *upperdir)
{
	 
	if (workdir == upperdir)
		goto err;

	 
	if (lock_rename(workdir, upperdir) != NULL)
		goto err_unlock;

	return 0;

err_unlock:
	unlock_rename(workdir, upperdir);
err:
	pr_err("overlayfs: failed to lock workdir+upperdir\n");
	return -EIO;
}
