xfs_da3_root_join(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*root_blk)
{
	struct xfs_da_intnode	*oldroot;
	struct xfs_da_args	*args;
	xfs_dablk_t		child;
	struct xfs_buf		*bp;
	struct xfs_da3_icnode_hdr oldroothdr;
	struct xfs_da_node_entry *btree;
	int			error;
	struct xfs_inode	*dp = state->args->dp;

	trace_xfs_da_root_join(state->args);

	ASSERT(root_blk->magic == XFS_DA_NODE_MAGIC);

	args = state->args;
	oldroot = root_blk->bp->b_addr;
	dp->d_ops->node_hdr_from_disk(&oldroothdr, oldroot);
	ASSERT(oldroothdr.forw == 0);
	ASSERT(oldroothdr.back == 0);

	 
	if (oldroothdr.count > 1)
		return 0;

	 
	btree = dp->d_ops->node_tree_p(oldroot);
	child = be32_to_cpu(btree[0].before);
	ASSERT(child != 0);
	error = xfs_da3_node_read(args->trans, dp, child, -1, &bp,
					     args->whichfork);
	if (error)
		return error;
	xfs_da_blkinfo_onlychild_validate(bp->b_addr, oldroothdr.level);

	 
	memcpy(root_blk->bp->b_addr, bp->b_addr, state->blocksize);
	root_blk->bp->b_ops = bp->b_ops;
	xfs_trans_buf_copy_type(root_blk->bp, bp);
	if (oldroothdr.magic == XFS_DA3_NODE_MAGIC) {
		struct xfs_da3_blkinfo *da3 = root_blk->bp->b_addr;
		da3->blkno = cpu_to_be64(root_blk->bp->b_bn);
	}
	xfs_trans_log_buf(args->trans, root_blk->bp, 0, state->blocksize - 1);
	error = xfs_da_shrink_inode(args, child, bp);
	return(error);
}
