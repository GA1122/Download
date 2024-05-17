static struct dentry *vfat_mount(struct file_system_type *fs_type,
		       int flags, const char *dev_name,
		       void *data)
{
	return mount_bdev(fs_type, flags, dev_name, data, vfat_fill_super);
}