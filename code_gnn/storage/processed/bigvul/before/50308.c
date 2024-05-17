static struct posix_acl *v9fs_get_cached_acl(struct inode *inode, int type)
{
	struct posix_acl *acl;
	 
	acl = get_cached_acl(inode, type);
	BUG_ON(is_uncached_acl(acl));
	return acl;
}
