static int may_linkat(struct path *link)
{
	struct inode *inode;

	if (!sysctl_protected_hardlinks)
		return 0;

	inode = link->dentry->d_inode;

	 
	if (safe_hardlink_source(inode) || inode_owner_or_capable(inode))
		return 0;

	audit_log_link_denied("linkat", link);
	return -EPERM;
}