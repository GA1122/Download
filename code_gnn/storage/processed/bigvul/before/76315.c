xfs_attr_fillstate(xfs_da_state_t *state)
{
	xfs_da_state_path_t *path;
	xfs_da_state_blk_t *blk;
	int level;

	trace_xfs_attr_fillstate(state->args);

	 
	path = &state->path;
	ASSERT((path->active >= 0) && (path->active < XFS_DA_NODE_MAXDEPTH));
	for (blk = path->blk, level = 0; level < path->active; blk++, level++) {
		if (blk->bp) {
			blk->disk_blkno = XFS_BUF_ADDR(blk->bp);
			blk->bp = NULL;
		} else {
			blk->disk_blkno = 0;
		}
	}

	 
	path = &state->altpath;
	ASSERT((path->active >= 0) && (path->active < XFS_DA_NODE_MAXDEPTH));
	for (blk = path->blk, level = 0; level < path->active; blk++, level++) {
		if (blk->bp) {
			blk->disk_blkno = XFS_BUF_ADDR(blk->bp);
			blk->bp = NULL;
		} else {
			blk->disk_blkno = 0;
		}
	}

	return 0;
}