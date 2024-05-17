 static struct dentry *hugetlbfs_mount(struct file_system_type *fs_type,
 	int flags, const char *dev_name, void *data)
 {
	return mount_nodev(fs_type, flags, data, hugetlbfs_fill_super);
}