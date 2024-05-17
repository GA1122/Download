static struct fuse_dev *fuse_get_dev(struct file *file)
{
	 
	return READ_ONCE(file->private_data);
}
