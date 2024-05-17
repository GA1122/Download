xfs_attr_remove_int(xfs_inode_t *dp, struct xfs_name *name, int flags)
{
	xfs_da_args_t	args;
	xfs_fsblock_t	firstblock;
	xfs_bmap_free_t	flist;
	int		error;
	xfs_mount_t	*mp = dp->i_mount;

	 
	memset((char *)&args, 0, sizeof(args));
	args.name = name->name;
	args.namelen = name->len;
	args.flags = flags;
	args.hashval = xfs_da_hashname(args.name, args.namelen);
	args.dp = dp;
	args.firstblock = &firstblock;
	args.flist = &flist;
	args.total = 0;
	args.whichfork = XFS_ATTR_FORK;

	 
	args.op_flags = XFS_DA_OP_OKNOENT;

	 
	error = xfs_qm_dqattach(dp, 0);
	if (error)
		return error;

	 
	args.trans = xfs_trans_alloc(mp, XFS_TRANS_ATTR_RM);

	 

	if (flags & ATTR_ROOT)
		args.trans->t_flags |= XFS_TRANS_RESERVE;

	error = xfs_trans_reserve(args.trans, &M_RES(mp)->tr_attrrm,
				  XFS_ATTRRM_SPACE_RES(mp), 0);
	if (error) {
		xfs_trans_cancel(args.trans, 0);
		return(error);
	}

	xfs_ilock(dp, XFS_ILOCK_EXCL);
	 
	xfs_trans_ijoin(args.trans, dp, 0);

	 
	if (!xfs_inode_hasattr(dp)) {
		error = XFS_ERROR(ENOATTR);
		goto out;
	}
	if (dp->i_d.di_aformat == XFS_DINODE_FMT_LOCAL) {
		ASSERT(dp->i_afp->if_flags & XFS_IFINLINE);
		error = xfs_attr_shortform_remove(&args);
		if (error) {
			goto out;
		}
	} else if (xfs_bmap_one_block(dp, XFS_ATTR_FORK)) {
		error = xfs_attr_leaf_removename(&args);
	} else {
		error = xfs_attr_node_removename(&args);
	}
	if (error) {
		goto out;
	}

	 
	if (mp->m_flags & XFS_MOUNT_WSYNC) {
		xfs_trans_set_sync(args.trans);
	}

	if ((flags & ATTR_KERNOTIME) == 0)
		xfs_trans_ichgtime(args.trans, dp, XFS_ICHGTIME_CHG);

	 
	xfs_trans_log_inode(args.trans, dp, XFS_ILOG_CORE);
	error = xfs_trans_commit(args.trans, XFS_TRANS_RELEASE_LOG_RES);
	xfs_iunlock(dp, XFS_ILOCK_EXCL);

	return(error);

out:
	if (args.trans)
		xfs_trans_cancel(args.trans,
			XFS_TRANS_RELEASE_LOG_RES|XFS_TRANS_ABORT);
	xfs_iunlock(dp, XFS_ILOCK_EXCL);
	return(error);
}
