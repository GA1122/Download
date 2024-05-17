cifs_d_revalidate(struct dentry *direntry, struct nameidata *nd)
{
	if (nd && (nd->flags & LOOKUP_RCU))
		return -ECHILD;

	if (direntry->d_inode) {
		if (cifs_revalidate_dentry(direntry))
			return 0;
		else {
			 
			if (IS_AUTOMOUNT(direntry->d_inode))
				return 0;
			return 1;
		}
	}

	 
	if (!nd)
		return 0;

	 
	if (nd->flags & (LOOKUP_CREATE | LOOKUP_RENAME_TARGET))
		return 0;

	if (time_after(jiffies, direntry->d_time + HZ) || !lookupCacheEnabled)
		return 0;

	return 1;
}
