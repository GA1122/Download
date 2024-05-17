ssize_t nfsd_copy_file_range(struct file *src, u64 src_pos, struct file *dst,
			     u64 dst_pos, u64 count)
{

	 
	count = min_t(u64, count, 1 << 22);
	return vfs_copy_file_range(src, src_pos, dst, dst_pos, count, 0);
}
