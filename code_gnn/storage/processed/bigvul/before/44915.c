xfs_attr_leaf_removename(xfs_da_args_t *args)
{
	xfs_inode_t *dp;
	struct xfs_buf *bp;
	int error, committed, forkoff;

	trace_xfs_attr_leaf_removename(args);

	 
	dp = args->dp;
	args->blkno = 0;
	error = xfs_attr3_leaf_read(args->trans, args->dp, args->blkno, -1, &bp);
	if (error)
		return error;

	error = xfs_attr3_leaf_lookup_int(bp, args);
	if (error == ENOATTR) {
		xfs_trans_brelse(args->trans, bp);
		return error;
	}

	xfs_attr3_leaf_remove(bp, args);

	 
	if ((forkoff = xfs_attr_shortform_allfit(bp, dp))) {
		xfs_bmap_init(args->flist, args->firstblock);
		error = xfs_attr3_leaf_to_shortform(bp, args, forkoff);
		 
		if (!error) {
			error = xfs_bmap_finish(&args->trans, args->flist,
						&committed);
		}
		if (error) {
			ASSERT(committed);
			args->trans = NULL;
			xfs_bmap_cancel(args->flist);
			return error;
		}

		 
		if (committed)
			xfs_trans_ijoin(args->trans, dp, 0);
	}
	return 0;
}
