xfs_attr3_leaf_add_work(
	struct xfs_buf		*bp,
	struct xfs_attr3_icleaf_hdr *ichdr,
	struct xfs_da_args	*args,
	int			mapindex)
{
	struct xfs_attr_leafblock *leaf;
	struct xfs_attr_leaf_entry *entry;
	struct xfs_attr_leaf_name_local *name_loc;
	struct xfs_attr_leaf_name_remote *name_rmt;
	struct xfs_mount	*mp;
	int			tmp;
	int			i;

	trace_xfs_attr_leaf_add_work(args);

	leaf = bp->b_addr;
	ASSERT(mapindex >= 0 && mapindex < XFS_ATTR_LEAF_MAPSIZE);
	ASSERT(args->index >= 0 && args->index <= ichdr->count);

	 
	entry = &xfs_attr3_leaf_entryp(leaf)[args->index];
	if (args->index < ichdr->count) {
		tmp  = ichdr->count - args->index;
		tmp *= sizeof(xfs_attr_leaf_entry_t);
		memmove(entry + 1, entry, tmp);
		xfs_trans_log_buf(args->trans, bp,
		    XFS_DA_LOGRANGE(leaf, entry, tmp + sizeof(*entry)));
	}
	ichdr->count++;

	 
	mp = args->trans->t_mountp;
	ASSERT(ichdr->freemap[mapindex].base < args->geo->blksize);
	ASSERT((ichdr->freemap[mapindex].base & 0x3) == 0);
	ASSERT(ichdr->freemap[mapindex].size >=
		xfs_attr_leaf_newentsize(args, NULL));
	ASSERT(ichdr->freemap[mapindex].size < args->geo->blksize);
	ASSERT((ichdr->freemap[mapindex].size & 0x3) == 0);

	ichdr->freemap[mapindex].size -= xfs_attr_leaf_newentsize(args, &tmp);

	entry->nameidx = cpu_to_be16(ichdr->freemap[mapindex].base +
				     ichdr->freemap[mapindex].size);
	entry->hashval = cpu_to_be32(args->hashval);
	entry->flags = tmp ? XFS_ATTR_LOCAL : 0;
	entry->flags |= XFS_ATTR_NSP_ARGS_TO_ONDISK(args->flags);
	if (args->op_flags & XFS_DA_OP_RENAME) {
		entry->flags |= XFS_ATTR_INCOMPLETE;
		if ((args->blkno2 == args->blkno) &&
		    (args->index2 <= args->index)) {
			args->index2++;
		}
	}
	xfs_trans_log_buf(args->trans, bp,
			  XFS_DA_LOGRANGE(leaf, entry, sizeof(*entry)));
	ASSERT((args->index == 0) ||
	       (be32_to_cpu(entry->hashval) >= be32_to_cpu((entry-1)->hashval)));
	ASSERT((args->index == ichdr->count - 1) ||
	       (be32_to_cpu(entry->hashval) <= be32_to_cpu((entry+1)->hashval)));

	 
	if (entry->flags & XFS_ATTR_LOCAL) {
		name_loc = xfs_attr3_leaf_name_local(leaf, args->index);
		name_loc->namelen = args->namelen;
		name_loc->valuelen = cpu_to_be16(args->valuelen);
		memcpy((char *)name_loc->nameval, args->name, args->namelen);
		memcpy((char *)&name_loc->nameval[args->namelen], args->value,
				   be16_to_cpu(name_loc->valuelen));
	} else {
		name_rmt = xfs_attr3_leaf_name_remote(leaf, args->index);
		name_rmt->namelen = args->namelen;
		memcpy((char *)name_rmt->name, args->name, args->namelen);
		entry->flags |= XFS_ATTR_INCOMPLETE;
		 
		name_rmt->valuelen = 0;
		name_rmt->valueblk = 0;
		args->rmtblkno = 1;
		args->rmtblkcnt = xfs_attr3_rmt_blocks(mp, args->valuelen);
		args->rmtvaluelen = args->valuelen;
	}
	xfs_trans_log_buf(args->trans, bp,
	     XFS_DA_LOGRANGE(leaf, xfs_attr3_leaf_name(leaf, args->index),
				   xfs_attr_leaf_entsize(leaf, args->index)));

	 
	if (be16_to_cpu(entry->nameidx) < ichdr->firstused)
		ichdr->firstused = be16_to_cpu(entry->nameidx);

	ASSERT(ichdr->firstused >= ichdr->count * sizeof(xfs_attr_leaf_entry_t)
					+ xfs_attr3_leaf_hdr_size(leaf));
	tmp = (ichdr->count - 1) * sizeof(xfs_attr_leaf_entry_t)
					+ xfs_attr3_leaf_hdr_size(leaf);

	for (i = 0; i < XFS_ATTR_LEAF_MAPSIZE; i++) {
		if (ichdr->freemap[i].base == tmp) {
			ichdr->freemap[i].base += sizeof(xfs_attr_leaf_entry_t);
			ichdr->freemap[i].size -= sizeof(xfs_attr_leaf_entry_t);
		}
	}
	ichdr->usedbytes += xfs_attr_leaf_entsize(leaf, args->index);
	return 0;
}