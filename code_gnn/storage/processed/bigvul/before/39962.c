int cifs_close(struct inode *inode, struct file *file)
{
	if (file->private_data != NULL) {
		cifsFileInfo_put(file->private_data);
		file->private_data = NULL;
	}

	 
	return 0;
}