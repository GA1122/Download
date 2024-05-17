xfs_bioerror(
	xfs_buf_t *bp)
{
#ifdef XFSERRORDEBUG
	ASSERT(XFS_BUF_ISREAD(bp) || bp->b_iodone);
#endif

	 
	xfs_buf_ioerror(bp, EIO);

	 
	XFS_BUF_UNREAD(bp);
	XFS_BUF_UNDONE(bp);
	xfs_buf_stale(bp);

	xfs_buf_ioend(bp, 0);

	return EIO;
}
