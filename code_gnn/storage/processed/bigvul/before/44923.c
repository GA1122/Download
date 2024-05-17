xfs_attr_set_int(
	struct xfs_inode *dp,
	struct xfs_name	*name,
	unsigned char	*value,
	int		valuelen,
	int		flags)
{
	xfs_da_args_t		args;
	xfs_fsblock_t		firstblock;
	xfs_bmap_free_t		flist;
	int			error, err2, committed;
	struct xfs_mount	*mp = dp->i_mount;
	struct xfs_trans_res	tres;
	int			rsvd = (flags & ATTR_ROOT) != 0;
	int			local;

	 
	error = xfs_qm_dqattach(dp, 0);
	if (error)
		return error;

	 
	if (XFS_IFORK_Q(dp) == 0) {
		int sf_size = sizeof(xfs_attr_sf_hdr_t) +
			      XFS_ATTR_SF_ENTSIZE_BYNAME(name->len, valuelen);

		if ((error = xfs_bmap_add_attrfork(dp, sf_size, rsvd)))
			return(error);
	}

	 
	memset((char *)&args, 0, sizeof(args));
	args.name = name->name;
	args.namelen = name->len;
	args.value = value;
	args.valuelen = valuelen;
	args.flags = flags;
	args.hashval = xfs_da_hashname(args.name, args.namelen);
	args.dp = dp;
	args.firstblock = &firstblock;
	args.flist = &flist;
	args.whichfork = XFS_ATTR_FORK;
	args.op_flags = XFS_DA_OP_ADDNAME | XFS_DA_OP_OKNOENT;

	 
	args.total = xfs_attr_calc_size(dp, name->len, valuelen, &local);

	 
	args.trans = xfs_trans_alloc(mp, XFS_TRANS_ATTR_SET);

	 

	if (rsvd)
		args.trans->t_flags |= XFS_TRANS_RESERVE;

	tres.tr_logres = M_RES(mp)->tr_attrsetm.tr_logres +
			 M_RES(mp)->tr_attrsetrt.tr_logres * args.total;
	tres.tr_logcount = XFS_ATTRSET_LOG_COUNT;
	tres.tr_logflags = XFS_TRANS_PERM_LOG_RES;
	error = xfs_trans_reserve(args.trans, &tres, args.total, 0);
	if (error) {
		xfs_trans_cancel(args.trans, 0);
		return(error);
	}
	xfs_ilock(dp, XFS_ILOCK_EXCL);

	error = xfs_trans_reserve_quota_nblks(args.trans, dp, args.total, 0,
				rsvd ? XFS_QMOPT_RES_REGBLKS | XFS_QMOPT_FORCE_RES :
				       XFS_QMOPT_RES_REGBLKS);
	if (error) {
		xfs_iunlock(dp, XFS_ILOCK_EXCL);
		xfs_trans_cancel(args.trans, XFS_TRANS_RELEASE_LOG_RES);
		return (error);
	}

	xfs_trans_ijoin(args.trans, dp, 0);

	 
	if ((dp->i_d.di_aformat == XFS_DINODE_FMT_LOCAL) ||
	    ((dp->i_d.di_aformat == XFS_DINODE_FMT_EXTENTS) &&
	     (dp->i_d.di_anextents == 0))) {

		 
		if (dp->i_d.di_aformat == XFS_DINODE_FMT_EXTENTS)
			xfs_attr_shortform_create(&args);

		 
		error = xfs_attr_shortform_addname(&args);
		if (error != ENOSPC) {
			 
			ASSERT(args.trans != NULL);

			 
			if (mp->m_flags & XFS_MOUNT_WSYNC) {
				xfs_trans_set_sync(args.trans);
			}

			if (!error && (flags & ATTR_KERNOTIME) == 0) {
				xfs_trans_ichgtime(args.trans, dp,
							XFS_ICHGTIME_CHG);
			}
			err2 = xfs_trans_commit(args.trans,
						 XFS_TRANS_RELEASE_LOG_RES);
			xfs_iunlock(dp, XFS_ILOCK_EXCL);

			return(error == 0 ? err2 : error);
		}

		 
		xfs_bmap_init(args.flist, args.firstblock);
		error = xfs_attr_shortform_to_leaf(&args);
		if (!error) {
			error = xfs_bmap_finish(&args.trans, args.flist,
						&committed);
		}
		if (error) {
			ASSERT(committed);
			args.trans = NULL;
			xfs_bmap_cancel(&flist);
			goto out;
		}

		 
		if (committed)
			xfs_trans_ijoin(args.trans, dp, 0);

		 

		error = xfs_trans_roll(&args.trans, dp);
		if (error)
			goto out;

	}

	if (xfs_bmap_one_block(dp, XFS_ATTR_FORK)) {
		error = xfs_attr_leaf_addname(&args);
	} else {
		error = xfs_attr_node_addname(&args);
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
