xfs_end_io_direct_write(
	struct kiocb		*iocb,
	loff_t			offset,
	ssize_t			size,
	void			*private)
{
	struct inode		*inode = file_inode(iocb->ki_filp);
	struct xfs_inode	*ip = XFS_I(inode);
	uintptr_t		flags = (uintptr_t)private;
	int			error = 0;

	trace_xfs_end_io_direct_write(ip, offset, size);

	if (XFS_FORCED_SHUTDOWN(ip->i_mount))
		return -EIO;

	if (size <= 0)
		return size;

	 
	if (flags == 0) {
		ASSERT(offset + size <= i_size_read(inode));
		return 0;
	}

	 
	spin_lock(&ip->i_flags_lock);
	if (offset + size > i_size_read(inode))
		i_size_write(inode, offset + size);
	spin_unlock(&ip->i_flags_lock);

	if (flags & XFS_DIO_FLAG_COW)
		error = xfs_reflink_end_cow(ip, offset, size);
	if (flags & XFS_DIO_FLAG_UNWRITTEN) {
		trace_xfs_end_io_direct_write_unwritten(ip, offset, size);

		error = xfs_iomap_write_unwritten(ip, offset, size);
	}
	if (flags & XFS_DIO_FLAG_APPEND) {
		trace_xfs_end_io_direct_write_append(ip, offset, size);

		error = xfs_setfilesize(ip, offset, size);
	}

	return error;
}
