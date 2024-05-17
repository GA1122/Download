xfs_setfilesize_trans_alloc(
	struct xfs_ioend	*ioend)
{
	struct xfs_mount	*mp = XFS_I(ioend->io_inode)->i_mount;
	struct xfs_trans	*tp;
	int			error;

	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_fsyncts, 0, 0, 0, &tp);
	if (error)
		return error;

	ioend->io_append_trans = tp;

	 
	__sb_writers_release(ioend->io_inode->i_sb, SB_FREEZE_FS);
	 
	current_restore_flags_nested(&tp->t_pflags, PF_FSTRANS);
	return 0;
}
