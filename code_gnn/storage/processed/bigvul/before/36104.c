static struct dentry *isofs_mount(struct file_system_type *fs_type,
	int flags, const char *dev_name, void *data)
{
	 
	if (!(flags & MS_RDONLY))
		return ERR_PTR(-EACCES);
	return mount_bdev(fs_type, flags, dev_name, data, isofs_fill_super);
}
