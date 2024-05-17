xfs_da3_node_toosmall(
	struct xfs_da_state	*state,
	int			*action)
{
	struct xfs_da_intnode	*node;
	struct xfs_da_state_blk	*blk;
	struct xfs_da_blkinfo	*info;
	xfs_dablk_t		blkno;
	struct xfs_buf		*bp;
	struct xfs_da3_icnode_hdr nodehdr;
	int			count;
	int			forward;
	int			error;
	int			retval;
	int			i;
	struct xfs_inode	*dp = state->args->dp;

	trace_xfs_da_node_toosmall(state->args);

	 
	blk = &state->path.blk[ state->path.active-1 ];
	info = blk->bp->b_addr;
	node = (xfs_da_intnode_t *)info;
	dp->d_ops->node_hdr_from_disk(&nodehdr, node);
	if (nodehdr.count > (state->node_ents >> 1)) {
		*action = 0;	 
		return(0);	 
	}

	 
	if (nodehdr.count == 0) {
		 
		forward = (info->forw != 0);
		memcpy(&state->altpath, &state->path, sizeof(state->path));
		error = xfs_da3_path_shift(state, &state->altpath, forward,
						 0, &retval);
		if (error)
			return(error);
		if (retval) {
			*action = 0;
		} else {
			*action = 2;
		}
		return(0);
	}

	 
	count  = state->node_ents;
	count -= state->node_ents >> 2;
	count -= nodehdr.count;

	 
	forward = nodehdr.forw < nodehdr.back;
	for (i = 0; i < 2; forward = !forward, i++) {
		struct xfs_da3_icnode_hdr thdr;
		if (forward)
			blkno = nodehdr.forw;
		else
			blkno = nodehdr.back;
		if (blkno == 0)
			continue;
		error = xfs_da3_node_read(state->args->trans, dp,
					blkno, -1, &bp, state->args->whichfork);
		if (error)
			return(error);

		node = bp->b_addr;
		dp->d_ops->node_hdr_from_disk(&thdr, node);
		xfs_trans_brelse(state->args->trans, bp);

		if (count - thdr.count >= 0)
			break;	 
	}
	if (i >= 2) {
		*action = 0;
		return 0;
	}

	 
	memcpy(&state->altpath, &state->path, sizeof(state->path));
	if (blkno < blk->blkno) {
		error = xfs_da3_path_shift(state, &state->altpath, forward,
						 0, &retval);
	} else {
		error = xfs_da3_path_shift(state, &state->path, forward,
						 0, &retval);
	}
	if (error)
		return error;
	if (retval) {
		*action = 0;
		return 0;
	}
	*action = 1;
	return 0;
}
