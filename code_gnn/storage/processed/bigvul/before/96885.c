static ssize_t kernel_readv(struct file *file, const struct kvec *vec,
			    unsigned long vlen, loff_t offset)
{
	mm_segment_t old_fs;
	loff_t pos = offset;
	ssize_t res;

	old_fs = get_fs();
	set_fs(KERNEL_DS);
	 
	res = vfs_readv(file, (const struct iovec __user *)vec, vlen, &pos, 0);
	set_fs(old_fs);

	return res;
}
