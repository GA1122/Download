xfs_attr_list_int(
	xfs_attr_list_context_t *context)
{
	int error;
	xfs_inode_t *dp = context->dp;
	uint		lock_mode;

	XFS_STATS_INC(xs_attr_list);

	if (XFS_FORCED_SHUTDOWN(dp->i_mount))
		return EIO;

	 
	lock_mode = xfs_ilock_attr_map_shared(dp);
	if (!xfs_inode_hasattr(dp)) {
		error = 0;
	} else if (dp->i_d.di_aformat == XFS_DINODE_FMT_LOCAL) {
		error = xfs_attr_shortform_list(context);
	} else if (xfs_bmap_one_block(dp, XFS_ATTR_FORK)) {
		error = xfs_attr_leaf_list(context);
	} else {
		error = xfs_attr_node_list(context);
	}
	xfs_iunlock(dp, lock_mode);
	return error;
}