xfs_setattr_size(
	struct xfs_inode	*ip,
	struct iattr		*iattr)
{
	struct xfs_mount	*mp = ip->i_mount;
	struct inode		*inode = VFS_I(ip);
	xfs_off_t		oldsize, newsize;
	struct xfs_trans	*tp;
	int			error;
	uint			lock_flags = 0;
	bool			did_zeroing = false;

	ASSERT(xfs_isilocked(ip, XFS_IOLOCK_EXCL));
	ASSERT(xfs_isilocked(ip, XFS_MMAPLOCK_EXCL));
	ASSERT(S_ISREG(inode->i_mode));
	ASSERT((iattr->ia_valid & (ATTR_UID|ATTR_GID|ATTR_ATIME|ATTR_ATIME_SET|
		ATTR_MTIME_SET|ATTR_KILL_PRIV|ATTR_TIMES_SET)) == 0);

	oldsize = inode->i_size;
	newsize = iattr->ia_size;

	 
	if (newsize == 0 && oldsize == 0 && ip->i_d.di_nextents == 0) {
		if (!(iattr->ia_valid & (ATTR_CTIME|ATTR_MTIME)))
			return 0;

		 
		iattr->ia_valid &= ~ATTR_SIZE;
		return xfs_setattr_nonsize(ip, iattr, 0);
	}

	 
	error = xfs_qm_dqattach(ip);
	if (error)
		return error;

	 
	inode_dio_wait(inode);

	 
	if (newsize > oldsize) {
		trace_xfs_zero_eof(ip, oldsize, newsize - oldsize);
		error = iomap_zero_range(inode, oldsize, newsize - oldsize,
				&did_zeroing, &xfs_iomap_ops);
	} else {
		error = iomap_truncate_page(inode, newsize, &did_zeroing,
				&xfs_iomap_ops);
	}

	if (error)
		return error;

	 
	truncate_setsize(inode, newsize);

	 
	if (did_zeroing ||
	    (newsize > ip->i_d.di_size && oldsize != ip->i_d.di_size)) {
		error = filemap_write_and_wait_range(VFS_I(ip)->i_mapping,
						ip->i_d.di_size, newsize - 1);
		if (error)
			return error;
	}

	error = xfs_trans_alloc(mp, &M_RES(mp)->tr_itruncate, 0, 0, 0, &tp);
	if (error)
		return error;

	lock_flags |= XFS_ILOCK_EXCL;
	xfs_ilock(ip, XFS_ILOCK_EXCL);
	xfs_trans_ijoin(tp, ip, 0);

	 
	if (newsize != oldsize &&
	    !(iattr->ia_valid & (ATTR_CTIME | ATTR_MTIME))) {
		iattr->ia_ctime = iattr->ia_mtime =
			current_time(inode);
		iattr->ia_valid |= ATTR_CTIME | ATTR_MTIME;
	}

	 
	ip->i_d.di_size = newsize;
	xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);

	if (newsize <= oldsize) {
		error = xfs_itruncate_extents(&tp, ip, XFS_DATA_FORK, newsize);
		if (error)
			goto out_trans_cancel;

		 
		xfs_iflags_set(ip, XFS_ITRUNCATED);

		 
		xfs_inode_clear_eofblocks_tag(ip);
	}

	if (iattr->ia_valid & ATTR_MODE)
		xfs_setattr_mode(ip, iattr);
	if (iattr->ia_valid & (ATTR_ATIME|ATTR_CTIME|ATTR_MTIME))
		xfs_setattr_time(ip, iattr);

	xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);

	XFS_STATS_INC(mp, xs_ig_attrchg);

	if (mp->m_flags & XFS_MOUNT_WSYNC)
		xfs_trans_set_sync(tp);

	error = xfs_trans_commit(tp);
out_unlock:
	if (lock_flags)
		xfs_iunlock(ip, lock_flags);
	return error;

out_trans_cancel:
	xfs_trans_cancel(tp);
	goto out_unlock;
}
