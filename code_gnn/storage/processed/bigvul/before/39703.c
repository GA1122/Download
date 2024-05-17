static int open_will_truncate(int flag, struct inode *inode)
{
	 
	if (special_file(inode->i_mode))
		return 0;
	return (flag & O_TRUNC);
}
