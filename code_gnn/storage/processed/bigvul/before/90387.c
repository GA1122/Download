static int megasas_mgmt_open(struct inode *inode, struct file *filep)
{
	 
	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;

	return 0;
}
