xfs_attr3_rmt_read_verify(
	struct xfs_buf	*bp)
{
	struct xfs_mount *mp = bp->b_target->bt_mount;
	char		*ptr;
	int		len;
	xfs_daddr_t	bno;

	 
	if (!xfs_sb_version_hascrc(&mp->m_sb))
		return;

	ptr = bp->b_addr;
	bno = bp->b_bn;
	len = BBTOB(bp->b_length);
	ASSERT(len >= XFS_LBSIZE(mp));

	while (len > 0) {
		if (!xfs_verify_cksum(ptr, XFS_LBSIZE(mp),
				      XFS_ATTR3_RMT_CRC_OFF)) {
			xfs_buf_ioerror(bp, EFSBADCRC);
			break;
		}
		if (!xfs_attr3_rmt_verify(mp, ptr, XFS_LBSIZE(mp), bno)) {
			xfs_buf_ioerror(bp, EFSCORRUPTED);
			break;
		}
		len -= XFS_LBSIZE(mp);
		ptr += XFS_LBSIZE(mp);
		bno += mp->m_bsize;
	}

	if (bp->b_error)
		xfs_verifier_error(bp);
	else
		ASSERT(len == 0);
}