xfs_attr_leaf_addname(xfs_da_args_t *args)
{
	xfs_inode_t *dp;
	struct xfs_buf *bp;
	int retval, error, forkoff;

	trace_xfs_attr_leaf_addname(args);

	 
	dp = args->dp;
	args->blkno = 0;
	error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno, -1, &bp);
	if (error)
		return error;

	 
	retval = xfs_attr3_leaf_lookup_int(bp, args);
	if ((args->flags & ATTR_REPLACE) && (retval == -ENOATTR)) {
		xfs_trans_brelse(args->trans, bp);
		return retval;
	} else if (retval == -EEXIST) {
		if (args->flags & ATTR_CREATE) {	 
			xfs_trans_brelse(args->trans, bp);
			return retval;
		}

		trace_xfs_attr_leaf_replace(args);

		 
		args->op_flags |= XFS_DA_OP_RENAME;	 
		args->blkno2 = args->blkno;		 
		args->index2 = args->index;
		args->rmtblkno2 = args->rmtblkno;
		args->rmtblkcnt2 = args->rmtblkcnt;
		args->rmtvaluelen2 = args->rmtvaluelen;

		 
		args->rmtblkno = 0;
		args->rmtblkcnt = 0;
		args->rmtvaluelen = 0;
	}

	 
	retval = xfs_attr3_leaf_add(bp, args);
	if (retval == -ENOSPC) {
		 
		xfs_defer_init(args->dfops, args->firstblock);
		error = xfs_attr3_leaf_to_node(args);
		if (error)
			goto out_defer_cancel;
		xfs_defer_ijoin(args->dfops, dp);
		error = xfs_defer_finish(&args->trans, args->dfops);
		if (error)
			goto out_defer_cancel;

		 
		error = xfs_trans_roll_inode(&args->trans, dp);
		if (error)
			return error;

		 
		error = xfs_attr_node_addname(args);
		return error;
	}

	 
	error = xfs_trans_roll_inode(&args->trans, dp);
	if (error)
		return error;

	 
	if (args->rmtblkno > 0) {
		error = xfs_attr_rmtval_set(args);
		if (error)
			return error;
	}

	 
	if (args->op_flags & XFS_DA_OP_RENAME) {
		 
		error = xfs_attr3_leaf_flipflags(args);
		if (error)
			return error;

		 
		args->index = args->index2;
		args->blkno = args->blkno2;
		args->rmtblkno = args->rmtblkno2;
		args->rmtblkcnt = args->rmtblkcnt2;
		args->rmtvaluelen = args->rmtvaluelen2;
		if (args->rmtblkno) {
			error = xfs_attr_rmtval_remove(args);
			if (error)
				return error;
		}

		 
		error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno,
					   -1, &bp);
		if (error)
			return error;

		xfs_attr3_leaf_remove(bp, args);

		 
		if ((forkoff = xfs_attr_shortform_allfit(bp, dp))) {
			xfs_defer_init(args->dfops, args->firstblock);
			error = xfs_attr3_leaf_to_shortform(bp, args, forkoff);
			 
			if (error)
				goto out_defer_cancel;
			xfs_defer_ijoin(args->dfops, dp);
			error = xfs_defer_finish(&args->trans, args->dfops);
			if (error)
				goto out_defer_cancel;
		}

		 
		error = xfs_trans_roll_inode(&args->trans, dp);

	} else if (args->rmtblkno > 0) {
		 
		error = xfs_attr3_leaf_clearflag(args);
	}
	return error;
out_defer_cancel:
	xfs_defer_cancel(args->dfops);
	return error;
}
