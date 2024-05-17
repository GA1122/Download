force_reval_path(struct path *path, struct nameidata *nd)
{
	int status;
	struct dentry *dentry = path->dentry;

	 
	if (!(dentry->d_sb->s_type->fs_flags & FS_REVAL_DOT))
		return 0;

	status = dentry->d_op->d_revalidate(dentry, nd);
	if (status > 0)
		return 0;

	if (!status) {
		d_invalidate(dentry);
		status = -ESTALE;
	}
	return status;
}
