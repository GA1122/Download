ssize_t kernel_write(struct file *file, const char *buf, size_t count,
			    loff_t pos)
{
	mm_segment_t old_fs;
	ssize_t res;

	old_fs = get_fs();
	set_fs(get_ds());
	 
	res = vfs_write(file, (__force const char __user *)buf, count, &pos);
	set_fs(old_fs);

	return res;
}
