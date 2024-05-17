int btrfs_init_acl(struct btrfs_trans_handle *trans,
		   struct inode *inode, struct inode *dir)
{
	struct posix_acl *default_acl, *acl;
	int ret = 0;

	 
	if (!dir)
		return 0;

	ret = posix_acl_create(dir, &inode->i_mode, &default_acl, &acl);
	if (ret)
		return ret;

	if (default_acl) {
		ret = __btrfs_set_acl(trans, inode, default_acl,
				      ACL_TYPE_DEFAULT);
		posix_acl_release(default_acl);
	}

	if (acl) {
		if (!ret)
			ret = __btrfs_set_acl(trans, inode, acl,
					      ACL_TYPE_ACCESS);
		posix_acl_release(acl);
	}

	if (!default_acl && !acl)
		cache_no_acl(inode);
	return ret;
}
