static int vfat_revalidate_ci(struct dentry *dentry, struct nameidata *nd)
{
	if (nd && nd->flags & LOOKUP_RCU)
		return -ECHILD;

	 
	if (dentry->d_inode)
		return 1;

	 
	if (!nd)
		return 0;

	 
	if (nd->flags & (LOOKUP_CREATE | LOOKUP_RENAME_TARGET))
		return 0;

	return vfat_revalidate_shortname(dentry);
}
