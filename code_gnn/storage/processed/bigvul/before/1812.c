static struct posix_acl *jffs2_iget_acl(struct inode *inode, struct posix_acl **i_acl)
{
	struct posix_acl *acl = JFFS2_ACL_NOT_CACHED;

	spin_lock(&inode->i_lock);
	if (*i_acl != JFFS2_ACL_NOT_CACHED)
		acl = posix_acl_dup(*i_acl);
	spin_unlock(&inode->i_lock);
	return acl;
}
