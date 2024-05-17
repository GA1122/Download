int hfsplus_init_posix_acl(struct inode *inode, struct inode *dir)
{
	int err = 0;
	struct posix_acl *default_acl, *acl;

	hfs_dbg(ACL_MOD,
		"[%s]: ino %lu, dir->ino %lu\n",
		__func__, inode->i_ino, dir->i_ino);

	if (S_ISLNK(inode->i_mode))
		return 0;

	err = posix_acl_create(dir, &inode->i_mode, &default_acl, &acl);
	if (err)
		return err;

	if (default_acl) {
		err = hfsplus_set_posix_acl(inode, default_acl,
					    ACL_TYPE_DEFAULT);
		posix_acl_release(default_acl);
	}

	if (acl) {
		if (!err)
			err = hfsplus_set_posix_acl(inode, acl,
						    ACL_TYPE_ACCESS);
		posix_acl_release(acl);
	}
	return err;
}
