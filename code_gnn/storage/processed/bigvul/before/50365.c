int orangefs_init_acl(struct inode *inode, struct inode *dir)
{
	struct orangefs_inode_s *orangefs_inode = ORANGEFS_I(inode);
	struct posix_acl *default_acl, *acl;
	umode_t mode = inode->i_mode;
	int error = 0;

	ClearModeFlag(orangefs_inode);

	error = posix_acl_create(dir, &mode, &default_acl, &acl);
	if (error)
		return error;

	if (default_acl) {
		error = orangefs_set_acl(inode, default_acl, ACL_TYPE_DEFAULT);
		posix_acl_release(default_acl);
	}

	if (acl) {
		if (!error)
			error = orangefs_set_acl(inode, acl, ACL_TYPE_ACCESS);
		posix_acl_release(acl);
	}

	 
	if (mode != inode->i_mode) {
		SetModeFlag(orangefs_inode);
		inode->i_mode = mode;
		orangefs_flush_inode(inode);
	}

	return error;
}