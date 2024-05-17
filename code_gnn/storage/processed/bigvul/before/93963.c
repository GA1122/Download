xfs_setfilesize_ioend(
	struct xfs_ioend	*ioend,
	int			error)
{
	struct xfs_inode	*ip = XFS_I(ioend->io_inode);
	struct xfs_trans	*tp = ioend->io_append_trans;

	 
	current_set_flags_nested(&tp->t_pflags, PF_FSTRANS);
	__sb_writers_acquired(VFS_I(ip)->i_sb, SB_FREEZE_FS);

	 
	if (error) {
		xfs_trans_cancel(tp);
		return error;
	}

	return __xfs_setfilesize(ip, tp, ioend->io_offset, ioend->io_size);
}
