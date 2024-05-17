int jfs_init_acl(tid_t tid, struct inode *inode, struct inode *dir)
{
	struct posix_acl *default_acl, *acl;
	int rc = 0;

	rc = posix_acl_create(dir, &inode->i_mode, &default_acl, &acl);
	if (rc)
		return rc;

	if (default_acl) {
		rc = __jfs_set_acl(tid, inode, ACL_TYPE_DEFAULT, default_acl);
		posix_acl_release(default_acl);
	}

	if (acl) {
		if (!rc)
			rc = __jfs_set_acl(tid, inode, ACL_TYPE_ACCESS, acl);
		posix_acl_release(acl);
	}

	JFS_IP(inode)->mode2 = (JFS_IP(inode)->mode2 & 0xffff0000) |
			       inode->i_mode;

	return rc;
}
