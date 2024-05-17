int f2fs_set_acl(struct inode *inode, struct posix_acl *acl, int type)
{
	return __f2fs_set_acl(inode, type, acl, NULL);
}
