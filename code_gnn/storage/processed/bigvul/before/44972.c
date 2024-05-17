xfs_attr3_rmt_write_verify(
	struct xfs_buf	*bp)
{
	struct xfs_mount *mp = bp->b_target->bt_mount;
	struct xfs_buf_log_item	*bip = bp->b_fspriv;
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
		if (!xfs_attr3_rmt_verify(mp, ptr, XFS_LBSIZE(mp), bno)) {
			xfs_buf_ioerror(bp, EFSCORRUPTED);
			xfs_verifier_error(bp);
			return;
		}
		if (bip) {
			struct xfs_attr3_rmt_hdr *rmt;

			rmt = (struct xfs_attr3_rmt_hdr *)ptr;
			rmt->rm_lsn = cpu_to_be64(bip->bli_item.li_lsn);
		}
		xfs_update_cksum(ptr, XFS_LBSIZE(mp), XFS_ATTR3_RMT_CRC_OFF);

		len -= XFS_LBSIZE(mp);
		ptr += XFS_LBSIZE(mp);
		bno += mp->m_bsize;
	}
	ASSERT(len == 0);
}