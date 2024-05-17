static inline int may_follow_link(struct nameidata *nd)
{
	const struct inode *inode;
	const struct inode *parent;

	if (!sysctl_protected_symlinks)
		return 0;

	 
	inode = nd->stack[0].inode;
	if (uid_eq(current_cred()->fsuid, inode->i_uid))
		return 0;

	 
	parent = nd->inode;
	if ((parent->i_mode & (S_ISVTX|S_IWOTH)) != (S_ISVTX|S_IWOTH))
		return 0;

	 
	if (uid_eq(parent->i_uid, inode->i_uid))
		return 0;

	if (nd->flags & LOOKUP_RCU)
		return -ECHILD;

	audit_log_link_denied("follow_link", &nd->stack[0].link);
	return -EACCES;
}