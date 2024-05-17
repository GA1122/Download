static inline u64 file_mmap_size_max(struct file *file, struct inode *inode)
{
	if (S_ISREG(inode->i_mode))
		return MAX_LFS_FILESIZE;

	if (S_ISBLK(inode->i_mode))
		return MAX_LFS_FILESIZE;

	 
	if (file->f_mode & FMODE_UNSIGNED_OFFSET)
		return 0;

	 
	return ULONG_MAX;
}
