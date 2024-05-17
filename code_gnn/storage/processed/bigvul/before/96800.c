static int fuse_dev_open(struct inode *inode, struct file *file)
{
	 
	file->private_data = NULL;
	return 0;
}
