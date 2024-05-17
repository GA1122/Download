renew_parental_timestamps(struct dentry *direntry)
{
	 
	do {
		direntry->d_time = jiffies;
		direntry = direntry->d_parent;
	} while (!IS_ROOT(direntry));
}
