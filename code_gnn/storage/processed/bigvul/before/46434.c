xfs_file_aio_write_checks(
	struct file		*file,
	loff_t			*pos,
	size_t			*count,
	int			*iolock)
{
	struct inode		*inode = file->f_mapping->host;
	struct xfs_inode	*ip = XFS_I(inode);
	int			error = 0;

restart:
	error = generic_write_checks(file, pos, count, S_ISBLK(inode->i_mode));
	if (error)
		return error;

	 
	if (*pos > i_size_read(inode)) {
		if (*iolock == XFS_IOLOCK_SHARED) {
			xfs_rw_iunlock(ip, *iolock);
			*iolock = XFS_IOLOCK_EXCL;
			xfs_rw_ilock(ip, *iolock);
			goto restart;
		}
		error = -xfs_zero_eof(ip, *pos, i_size_read(inode));
		if (error)
			return error;
	}

	 
	if (likely(!(file->f_mode & FMODE_NOCMTIME))) {
		error = file_update_time(file);
		if (error)
			return error;
	}

	 
	return file_remove_suid(file);
}
