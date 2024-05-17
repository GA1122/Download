xfs_attr_node_removename(xfs_da_args_t *args)
{
	xfs_da_state_t *state;
	xfs_da_state_blk_t *blk;
	xfs_inode_t *dp;
	struct xfs_buf *bp;
	int retval, error, committed, forkoff;

	trace_xfs_attr_node_removename(args);

	 
	dp = args->dp;
	state = xfs_da_state_alloc();
	state->args = args;
	state->mp = dp->i_mount;
	state->blocksize = state->mp->m_sb.sb_blocksize;
	state->node_ents = state->mp->m_attr_node_ents;

	 
	error = xfs_da3_node_lookup_int(state, &retval);
	if (error || (retval != EEXIST)) {
		if (error == 0)
			error = retval;
		goto out;
	}

	 
	blk = &state->path.blk[ state->path.active-1 ];
	ASSERT(blk->bp != NULL);
	ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC);
	if (args->rmtblkno > 0) {
		 
		error = xfs_attr_fillstate(state);
		if (error)
			goto out;

		 
		error = xfs_attr3_leaf_setflag(args);
		if (error)
			goto out;
		error = xfs_attr_rmtval_remove(args);
		if (error)
			goto out;

		 
		error = xfs_attr_refillstate(state);
		if (error)
			goto out;
	}

	 
	blk = &state->path.blk[ state->path.active-1 ];
	ASSERT(blk->magic == XFS_ATTR_LEAF_MAGIC);
	retval = xfs_attr3_leaf_remove(blk->bp, args);
	xfs_da3_fixhashpath(state, &state->path);

	 
	if (retval && (state->path.active > 1)) {
		xfs_bmap_init(args->flist, args->firstblock);
		error = xfs_da3_join(state);
		if (!error) {
			error = xfs_bmap_finish(&args->trans, args->flist,
						&committed);
		}
		if (error) {
			ASSERT(committed);
			args->trans = NULL;
			xfs_bmap_cancel(args->flist);
			goto out;
		}

		 
		if (committed)
			xfs_trans_ijoin(args->trans, dp, 0);

		 
		error = xfs_trans_roll(&args->trans, dp);
		if (error)
			goto out;
	}

	 
	if (xfs_bmap_one_block(dp, XFS_ATTR_FORK)) {
		 
		ASSERT(state->path.active == 1);
		ASSERT(state->path.blk[0].bp);
		state->path.blk[0].bp = NULL;

		error = xfs_attr3_leaf_read(args->trans, args->dp, 0, -1, &bp);
		if (error)
			goto out;

		if ((forkoff = xfs_attr_shortform_allfit(bp, dp))) {
			xfs_bmap_init(args->flist, args->firstblock);
			error = xfs_attr3_leaf_to_shortform(bp, args, forkoff);
			 
			if (!error) {
				error = xfs_bmap_finish(&args->trans,
							args->flist,
							&committed);
			}
			if (error) {
				ASSERT(committed);
				args->trans = NULL;
				xfs_bmap_cancel(args->flist);
				goto out;
			}

			 
			if (committed)
				xfs_trans_ijoin(args->trans, dp, 0);
		} else
			xfs_trans_brelse(args->trans, bp);
	}
	error = 0;

out:
	xfs_da_state_free(state);
	return(error);
}