xfs_map_cow(
	struct xfs_writepage_ctx *wpc,
	struct inode		*inode,
	loff_t			offset,
	unsigned int		*new_type)
{
	struct xfs_inode	*ip = XFS_I(inode);
	struct xfs_bmbt_irec	imap;
	bool			is_cow = false, need_alloc = false;
	int			error;

	 
	if (wpc->io_type == XFS_IO_COW) {
		wpc->imap_valid = xfs_imap_valid(inode, &wpc->imap, offset);
		if (wpc->imap_valid) {
			*new_type = XFS_IO_COW;
			return 0;
		}
	}

	 
	xfs_ilock(ip, XFS_ILOCK_SHARED);
	is_cow = xfs_reflink_find_cow_mapping(ip, offset, &imap, &need_alloc);
	xfs_iunlock(ip, XFS_ILOCK_SHARED);

	if (!is_cow)
		return 0;

	 
	if (need_alloc) {
		error = xfs_iomap_write_allocate(ip, XFS_COW_FORK, offset,
				&imap);
		if (error)
			return error;
	}

	wpc->io_type = *new_type = XFS_IO_COW;
	wpc->imap_valid = true;
	wpc->imap = imap;
	return 0;
}