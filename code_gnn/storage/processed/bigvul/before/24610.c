static struct fuse_conn *fuse_get_conn(struct file *file)
{
	 
	return file->private_data;
}
