xfs_attr3_leaf_add(
	struct xfs_buf		*bp,
	struct xfs_da_args	*args)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr3_icleaf_hdr ichdr;
	int			tablesize;
	int			entsize;
	int			sum;
	int			tmp;
	int			i;

	trace_xfs_attr_leaf_add(args);

	leaf = bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(args->geo, &ichdr, leaf);
	ASSERT(args->index >= 0 && args->index <= ichdr.count);
	entsize = xfs_attr_leaf_newentsize(args, NULL);

	 
	tablesize = (ichdr.count + 1) * sizeof(xfs_attr_leaf_entry_t)
					+ xfs_attr3_leaf_hdr_size(leaf);
	for (sum = 0, i = XFS_ATTR_LEAF_MAPSIZE - 1; i >= 0; i--) {
		if (tablesize > ichdr.firstused) {
			sum += ichdr.freemap[i].size;
			continue;
		}
		if (!ichdr.freemap[i].size)
			continue;	 
		tmp = entsize;
		if (ichdr.freemap[i].base < ichdr.firstused)
			tmp += sizeof(xfs_attr_leaf_entry_t);
		if (ichdr.freemap[i].size >= tmp) {
			tmp = xfs_attr3_leaf_add_work(bp, &ichdr, args, i);
			goto out_log_hdr;
		}
		sum += ichdr.freemap[i].size;
	}

	 
	if (!ichdr.holes && sum < entsize)
		return -ENOSPC;

	 
	xfs_attr3_leaf_compact(args, &ichdr, bp);

	 
	if (ichdr.freemap[0].size < (entsize + sizeof(xfs_attr_leaf_entry_t))) {
		tmp = -ENOSPC;
		goto out_log_hdr;
	}

	tmp = xfs_attr3_leaf_add_work(bp, &ichdr, args, 0);

out_log_hdr:
	xfs_attr3_leaf_hdr_to_disk(args->geo, leaf, &ichdr);
	xfs_trans_log_buf(args->trans, bp,
		XFS_DA_LOGRANGE(leaf, &leaf->hdr,
				xfs_attr3_leaf_hdr_size(leaf)));
	return tmp;
}