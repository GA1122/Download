static bool safe_hardlink_source(struct inode *inode)
{
	umode_t mode = inode->i_mode;

	 
	if (!S_ISREG(mode))
		return false;

	 
	if (mode & S_ISUID)
		return false;

	 
	if ((mode & (S_ISGID | S_IXGRP)) == (S_ISGID | S_IXGRP))
		return false;

	 
	if (inode_permission(inode, MAY_READ | MAY_WRITE))
		return false;

	return true;
}
