xfs_reclaim_inode(
	struct xfs_inode	*ip,
	struct xfs_perag	*pag,
	int			sync_mode)
{
	struct xfs_buf		*bp = NULL;
	xfs_ino_t		ino = ip->i_ino;  
	int			error;

restart:
	error = 0;
	xfs_ilock(ip, XFS_ILOCK_EXCL);
	if (!xfs_iflock_nowait(ip)) {
		if (!(sync_mode & SYNC_WAIT))
			goto out;
		xfs_iflock(ip);
	}

	if (XFS_FORCED_SHUTDOWN(ip->i_mount)) {
		xfs_iunpin_wait(ip);
		 
		xfs_iflush_abort(ip, false);
		goto reclaim;
	}
	if (xfs_ipincount(ip)) {
		if (!(sync_mode & SYNC_WAIT))
			goto out_ifunlock;
		xfs_iunpin_wait(ip);
	}
	if (xfs_iflags_test(ip, XFS_ISTALE) || xfs_inode_clean(ip)) {
		xfs_ifunlock(ip);
		goto reclaim;
	}

	 
	if (!(sync_mode & SYNC_WAIT))
		goto out_ifunlock;

	 
	error = xfs_iflush(ip, &bp);
	if (error == -EAGAIN) {
		xfs_iunlock(ip, XFS_ILOCK_EXCL);
		 
		delay(2);
		goto restart;
	}

	if (!error) {
		error = xfs_bwrite(bp);
		xfs_buf_relse(bp);
	}

reclaim:
	ASSERT(!xfs_isiflocked(ip));

	 
	spin_lock(&ip->i_flags_lock);
	ip->i_flags = XFS_IRECLAIM;
	ip->i_ino = 0;
	spin_unlock(&ip->i_flags_lock);

	xfs_iunlock(ip, XFS_ILOCK_EXCL);

	XFS_STATS_INC(ip->i_mount, xs_ig_reclaims);
	 
	spin_lock(&pag->pag_ici_lock);
	if (!radix_tree_delete(&pag->pag_ici_root,
				XFS_INO_TO_AGINO(ip->i_mount, ino)))
		ASSERT(0);
	xfs_perag_clear_reclaim_tag(pag);
	spin_unlock(&pag->pag_ici_lock);

	 
	xfs_ilock(ip, XFS_ILOCK_EXCL);
	xfs_qm_dqdetach(ip);
	xfs_iunlock(ip, XFS_ILOCK_EXCL);

	__xfs_inode_free(ip);
	return error;

out_ifunlock:
	xfs_ifunlock(ip);
out:
	xfs_iflags_clear(ip, XFS_IRECLAIM);
	xfs_iunlock(ip, XFS_ILOCK_EXCL);
	 
	return 0;
}
