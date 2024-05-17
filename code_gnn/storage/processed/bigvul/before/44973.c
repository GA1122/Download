xfs_attr_rmtval_copyin(
	struct xfs_mount *mp,
	struct xfs_buf	*bp,
	xfs_ino_t	ino,
	int		*offset,
	int		*valuelen,
	__uint8_t	**src)
{
	char		*dst = bp->b_addr;
	xfs_daddr_t	bno = bp->b_bn;
	int		len = BBTOB(bp->b_length);

	ASSERT(len >= XFS_LBSIZE(mp));

	while (len > 0 && *valuelen > 0) {
		int hdr_size;
		int byte_cnt = XFS_ATTR3_RMT_BUF_SPACE(mp, XFS_LBSIZE(mp));

		byte_cnt = min(*valuelen, byte_cnt);
		hdr_size = xfs_attr3_rmt_hdr_set(mp, dst, ino, *offset,
						 byte_cnt, bno);

		memcpy(dst + hdr_size, *src, byte_cnt);

		 
		if (byte_cnt + hdr_size < XFS_LBSIZE(mp)) {
			ASSERT(*valuelen - byte_cnt == 0);
			ASSERT(len == XFS_LBSIZE(mp));
			memset(dst + hdr_size + byte_cnt, 0,
					XFS_LBSIZE(mp) - hdr_size - byte_cnt);
		}

		 
		len -= XFS_LBSIZE(mp);
		dst += XFS_LBSIZE(mp);
		bno += mp->m_bsize;

		 
		*valuelen -= byte_cnt;
		*src += byte_cnt;
		*offset += byte_cnt;
	}
}