xfs_end_io(
	struct work_struct *work)
{
	struct xfs_ioend	*ioend =
		container_of(work, struct xfs_ioend, io_work);
	struct xfs_inode	*ip = XFS_I(ioend->io_inode);
	int			error = ioend->io_bio->bi_error;

	 
	if (XFS_FORCED_SHUTDOWN(ip->i_mount))
		error = -EIO;

	 
	if (ioend->io_type == XFS_IO_COW) {
		if (error)
			goto done;
		if (ioend->io_bio->bi_error) {
			error = xfs_reflink_cancel_cow_range(ip,
					ioend->io_offset, ioend->io_size);
			goto done;
		}
		error = xfs_reflink_end_cow(ip, ioend->io_offset,
				ioend->io_size);
		if (error)
			goto done;
	}

	 
	if (ioend->io_type == XFS_IO_UNWRITTEN) {
		if (error)
			goto done;
		error = xfs_iomap_write_unwritten(ip, ioend->io_offset,
						  ioend->io_size);
	} else if (ioend->io_append_trans) {
		error = xfs_setfilesize_ioend(ioend, error);
	} else {
		ASSERT(!xfs_ioend_is_append(ioend) ||
		       ioend->io_type == XFS_IO_COW);
	}

done:
	xfs_destroy_ioend(ioend, error);
}
