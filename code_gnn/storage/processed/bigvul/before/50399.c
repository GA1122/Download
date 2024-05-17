reiserfs_inherit_default_acl(struct reiserfs_transaction_handle *th,
			     struct inode *dir, struct dentry *dentry,
			     struct inode *inode)
{
	struct posix_acl *default_acl, *acl;
	int err = 0;

	 
	if (S_ISLNK(inode->i_mode))
		return 0;

	 
	if (get_inode_sd_version(dir) == STAT_DATA_V1)
		goto apply_umask;

	 
	if (IS_PRIVATE(dir)) {
		inode->i_flags |= S_PRIVATE;
		goto apply_umask;
	}

	err = posix_acl_create(dir, &inode->i_mode, &default_acl, &acl);
	if (err)
		return err;

	if (default_acl) {
		err = __reiserfs_set_acl(th, inode, ACL_TYPE_DEFAULT,
					 default_acl);
		posix_acl_release(default_acl);
	}
	if (acl) {
		if (!err)
			err = __reiserfs_set_acl(th, inode, ACL_TYPE_ACCESS,
						 acl);
		posix_acl_release(acl);
	}

	return err;

apply_umask:
	 
	inode->i_mode &= ~current_umask();
	return err;
}
