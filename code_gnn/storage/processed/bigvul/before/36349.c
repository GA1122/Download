static int may_linkat(struct path *link)
{
	const struct cred *cred;
	struct inode *inode;

	if (!sysctl_protected_hardlinks)
		return 0;

	cred = current_cred();
	inode = link->dentry->d_inode;

	 
	if (uid_eq(cred->fsuid, inode->i_uid) || safe_hardlink_source(inode) ||
	    capable(CAP_FOWNER))
		return 0;

	audit_log_link_denied("linkat", link);
	return -EPERM;
}