xfs_iread(
	xfs_mount_t	*mp,
	xfs_trans_t	*tp,
	xfs_inode_t	*ip,
	uint		iget_flags)
{
	xfs_buf_t	*bp;
	xfs_dinode_t	*dip;
	xfs_failaddr_t	fa;
	int		error;

	 
	error = xfs_imap(mp, tp, ip->i_ino, &ip->i_imap, iget_flags);
	if (error)
		return error;

	 
	if ((iget_flags & XFS_IGET_CREATE) &&
	    xfs_sb_version_hascrc(&mp->m_sb) &&
	    !(mp->m_flags & XFS_MOUNT_IKEEP)) {
		 
		memset(&ip->i_d, 0, sizeof(ip->i_d));
		VFS_I(ip)->i_generation = prandom_u32();
		ip->i_d.di_version = 3;
		return 0;
	}

	 
	error = xfs_imap_to_bp(mp, tp, &ip->i_imap, &dip, &bp, 0, iget_flags);
	if (error)
		return error;

	 
	fa = xfs_dinode_verify(mp, ip->i_ino, dip);
	if (fa) {
		xfs_inode_verifier_error(ip, -EFSCORRUPTED, "dinode", dip,
				sizeof(*dip), fa);
		error = -EFSCORRUPTED;
		goto out_brelse;
	}

	 
	if (dip->di_mode) {
		xfs_inode_from_disk(ip, dip);
		error = xfs_iformat_fork(ip, dip);
		if (error)  {
#ifdef DEBUG
			xfs_alert(mp, "%s: xfs_iformat() returned error %d",
				__func__, error);
#endif  
			goto out_brelse;
		}
	} else {
		 
		ip->i_d.di_version = dip->di_version;
		VFS_I(ip)->i_generation = be32_to_cpu(dip->di_gen);
		ip->i_d.di_flushiter = be16_to_cpu(dip->di_flushiter);

		 
		VFS_I(ip)->i_mode = 0;
	}

	ASSERT(ip->i_d.di_version >= 2);
	ip->i_delayed_blks = 0;

	 
	xfs_buf_set_ref(bp, XFS_INO_REF);

	 
 out_brelse:
	xfs_trans_brelse(tp, bp);
	return error;
}
