struct posix_acl *f2fs_get_acl(struct inode *inode, int type)
{
	return __f2fs_get_acl(inode, type, NULL);
}
