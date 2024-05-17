xfs_ioc_space(
	struct xfs_inode	*ip,
	struct inode		*inode,
	struct file		*filp,
	int			ioflags,
	unsigned int		cmd,
	xfs_flock64_t		*bf)
{
	struct xfs_mount	*mp = ip->i_mount;
	struct xfs_trans	*tp;
	struct iattr		iattr;
	bool			setprealloc = false;
	bool			clrprealloc = false;
	int			error;

	 
	if (!xfs_sb_version_hasextflgbit(&ip->i_mount->m_sb) &&
	    !capable(CAP_SYS_ADMIN))
		return -XFS_ERROR(EPERM);

	if (inode->i_flags & (S_IMMUTABLE|S_APPEND))
		return -XFS_ERROR(EPERM);

	if (!(filp->f_mode & FMODE_WRITE))
		return -XFS_ERROR(EBADF);

	if (!S_ISREG(inode->i_mode))
		return -XFS_ERROR(EINVAL);

	error = mnt_want_write_file(filp);
	if (error)
		return error;

	xfs_ilock(ip, XFS_IOLOCK_EXCL);

	switch (bf->l_whence) {
	case 0:  
		break;
	case 1:  
		bf->l_start += filp->f_pos;
		break;
	case 2:  
		bf->l_start += XFS_ISIZE(ip);
		break;
	default:
		error = XFS_ERROR(EINVAL);
		goto out_unlock;
	}

	 
	switch (cmd) {
	case XFS_IOC_ZERO_RANGE:
	case XFS_IOC_RESVSP:
	case XFS_IOC_RESVSP64:
	case XFS_IOC_UNRESVSP:
	case XFS_IOC_UNRESVSP64:
		if (bf->l_len <= 0) {
			error = XFS_ERROR(EINVAL);
			goto out_unlock;
		}
		break;
	default:
		bf->l_len = 0;
		break;
	}

	if (bf->l_start < 0 ||
	    bf->l_start > mp->m_super->s_maxbytes ||
	    bf->l_start + bf->l_len < 0 ||
	    bf->l_start + bf->l_len >= mp->m_super->s_maxbytes) {
		error = XFS_ERROR(EINVAL);
		goto out_unlock;
	}

	switch (cmd) {
	case XFS_IOC_ZERO_RANGE:
		error = xfs_zero_file_space(ip, bf->l_start, bf->l_len);
		if (!error)
			setprealloc = true;
		break;
	case XFS_IOC_RESVSP:
	case XFS_IOC_RESVSP64:
		error = xfs_alloc_file_space(ip, bf->l_start, bf->l_len,
						XFS_BMAPI_PREALLOC);
		if (!error)
			setprealloc = true;
		break;
	case XFS_IOC_UNRESVSP:
	case XFS_IOC_UNRESVSP64:
		error = xfs_free_file_space(ip, bf->l_start, bf->l_len);
		break;
	case XFS_IOC_ALLOCSP:
	case XFS_IOC_ALLOCSP64:
	case XFS_IOC_FREESP:
	case XFS_IOC_FREESP64:
		if (bf->l_start > XFS_ISIZE(ip)) {
			error = xfs_alloc_file_space(ip, XFS_ISIZE(ip),
					bf->l_start - XFS_ISIZE(ip), 0);
			if (error)
				goto out_unlock;
		}

		iattr.ia_valid = ATTR_SIZE;
		iattr.ia_size = bf->l_start;

		error = xfs_setattr_size(ip, &iattr);
		if (!error)
			clrprealloc = true;
		break;
	default:
		ASSERT(0);
		error = XFS_ERROR(EINVAL);
	}

	if (error)
		goto out_unlock;

	tp = xfs_trans_alloc(mp, XFS_TRANS_WRITEID);
	error = xfs_trans_reserve(tp, &M_RES(mp)->tr_writeid, 0, 0);
	if (error) {
		xfs_trans_cancel(tp, 0);
		goto out_unlock;
	}

	xfs_ilock(ip, XFS_ILOCK_EXCL);
	xfs_trans_ijoin(tp, ip, XFS_ILOCK_EXCL);

	if (!(ioflags & IO_INVIS)) {
		ip->i_d.di_mode &= ~S_ISUID;
		if (ip->i_d.di_mode & S_IXGRP)
			ip->i_d.di_mode &= ~S_ISGID;
		xfs_trans_ichgtime(tp, ip, XFS_ICHGTIME_MOD | XFS_ICHGTIME_CHG);
	}

	if (setprealloc)
		ip->i_d.di_flags |= XFS_DIFLAG_PREALLOC;
	else if (clrprealloc)
		ip->i_d.di_flags &= ~XFS_DIFLAG_PREALLOC;

	xfs_trans_log_inode(tp, ip, XFS_ILOG_CORE);
	if (filp->f_flags & O_DSYNC)
		xfs_trans_set_sync(tp);
	error = xfs_trans_commit(tp, 0);

out_unlock:
	xfs_iunlock(ip, XFS_IOLOCK_EXCL);
	mnt_drop_write_file(filp);
	return -error;
}