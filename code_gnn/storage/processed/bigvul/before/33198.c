xfs_bdstrat_cb(
	struct xfs_buf	*bp)
{
	if (XFS_FORCED_SHUTDOWN(bp->b_target->bt_mount)) {
		trace_xfs_bdstrat_shut(bp, _RET_IP_);
		 
		if (!bp->b_iodone && !XFS_BUF_ISREAD(bp))
			return xfs_bioerror_relse(bp);
		else
			return xfs_bioerror(bp);
	}

	xfs_buf_iorequest(bp);
	return 0;
}
