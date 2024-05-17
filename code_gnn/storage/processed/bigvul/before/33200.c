xfs_bioerror_relse(
	struct xfs_buf	*bp)
{
	int64_t		fl = bp->b_flags;
	 
	XFS_BUF_UNREAD(bp);
	XFS_BUF_DONE(bp);
	xfs_buf_stale(bp);
	bp->b_iodone = NULL;
	if (!(fl & XBF_ASYNC)) {
		 
		xfs_buf_ioerror(bp, EIO);
		complete(&bp->b_iowait);
	} else {
		xfs_buf_relse(bp);
	}

	return EIO;
}
