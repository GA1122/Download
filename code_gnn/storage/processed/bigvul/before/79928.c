xfs_attr3_leaf_unbalance(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*drop_blk,
	struct xfs_da_state_blk	*save_blk)
{
	struct xfs_attr_leafblock *drop_leaf = drop_blk->bp->b_addr;
	struct xfs_attr_leafblock *save_leaf = save_blk->bp->b_addr;
	struct xfs_attr3_icleaf_hdr drophdr;
	struct xfs_attr3_icleaf_hdr savehdr;
	struct xfs_attr_leaf_entry *entry;

	trace_xfs_attr_leaf_unbalance(state->args);

	drop_leaf = drop_blk->bp->b_addr;
	save_leaf = save_blk->bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(state->args->geo, &drophdr, drop_leaf);
	xfs_attr3_leaf_hdr_from_disk(state->args->geo, &savehdr, save_leaf);
	entry = xfs_attr3_leaf_entryp(drop_leaf);

	 
	drop_blk->hashval = be32_to_cpu(entry[drophdr.count - 1].hashval);

	 
	if (savehdr.holes == 0) {
		 
		if (xfs_attr3_leaf_order(save_blk->bp, &savehdr,
					 drop_blk->bp, &drophdr)) {
			xfs_attr3_leaf_moveents(state->args,
						drop_leaf, &drophdr, 0,
						save_leaf, &savehdr, 0,
						drophdr.count);
		} else {
			xfs_attr3_leaf_moveents(state->args,
						drop_leaf, &drophdr, 0,
						save_leaf, &savehdr,
						savehdr.count, drophdr.count);
		}
	} else {
		 
		struct xfs_attr_leafblock *tmp_leaf;
		struct xfs_attr3_icleaf_hdr tmphdr;

		tmp_leaf = kmem_zalloc(state->args->geo->blksize, KM_SLEEP);

		 
		memcpy(tmp_leaf, save_leaf, xfs_attr3_leaf_hdr_size(save_leaf));

		memset(&tmphdr, 0, sizeof(tmphdr));
		tmphdr.magic = savehdr.magic;
		tmphdr.forw = savehdr.forw;
		tmphdr.back = savehdr.back;
		tmphdr.firstused = state->args->geo->blksize;

		 
		xfs_attr3_leaf_hdr_to_disk(state->args->geo, tmp_leaf, &tmphdr);

		if (xfs_attr3_leaf_order(save_blk->bp, &savehdr,
					 drop_blk->bp, &drophdr)) {
			xfs_attr3_leaf_moveents(state->args,
						drop_leaf, &drophdr, 0,
						tmp_leaf, &tmphdr, 0,
						drophdr.count);
			xfs_attr3_leaf_moveents(state->args,
						save_leaf, &savehdr, 0,
						tmp_leaf, &tmphdr, tmphdr.count,
						savehdr.count);
		} else {
			xfs_attr3_leaf_moveents(state->args,
						save_leaf, &savehdr, 0,
						tmp_leaf, &tmphdr, 0,
						savehdr.count);
			xfs_attr3_leaf_moveents(state->args,
						drop_leaf, &drophdr, 0,
						tmp_leaf, &tmphdr, tmphdr.count,
						drophdr.count);
		}
		memcpy(save_leaf, tmp_leaf, state->args->geo->blksize);
		savehdr = tmphdr;  
		kmem_free(tmp_leaf);
	}

	xfs_attr3_leaf_hdr_to_disk(state->args->geo, save_leaf, &savehdr);
	xfs_trans_log_buf(state->args->trans, save_blk->bp, 0,
					   state->args->geo->blksize - 1);

	 
	entry = xfs_attr3_leaf_entryp(save_leaf);
	save_blk->hashval = be32_to_cpu(entry[savehdr.count - 1].hashval);
}
