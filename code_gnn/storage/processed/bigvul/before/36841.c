int file_remove_suid(struct file *file)
{
	struct dentry *dentry = file->f_path.dentry;
	struct inode *inode = dentry->d_inode;
	int killsuid;
	int killpriv;
	int error = 0;

	 
	if (IS_NOSEC(inode))
		return 0;

	killsuid = should_remove_suid(dentry);
	killpriv = security_inode_need_killpriv(dentry);

	if (killpriv < 0)
		return killpriv;
	if (killpriv)
		error = security_inode_killpriv(dentry);
	if (!error && killsuid)
		error = __remove_suid(dentry, killsuid);
	if (!error && (inode->i_sb->s_flags & MS_NOSEC))
		inode->i_flags |= S_NOSEC;

	return error;
}