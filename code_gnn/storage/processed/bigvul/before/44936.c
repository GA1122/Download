xfs_attr3_leaf_rebalance(
	struct xfs_da_state	*state,
	struct xfs_da_state_blk	*blk1,
	struct xfs_da_state_blk	*blk2)
{
	struct xfs_da_args	*args;
	struct xfs_attr_leafblock *leaf1;
	struct xfs_attr_leafblock *leaf2;
	struct xfs_attr3_icleaf_hdr ichdr1;
	struct xfs_attr3_icleaf_hdr ichdr2;
	struct xfs_attr_leaf_entry *entries1;
	struct xfs_attr_leaf_entry *entries2;
	int			count;
	int			totallen;
	int			max;
	int			space;
	int			swap;

	 
	ASSERT(blk1->magic == XFS_ATTR_LEAF_MAGIC);
	ASSERT(blk2->magic == XFS_ATTR_LEAF_MAGIC);
	leaf1 = blk1->bp->b_addr;
	leaf2 = blk2->bp->b_addr;
	xfs_attr3_leaf_hdr_from_disk(&ichdr1, leaf1);
	xfs_attr3_leaf_hdr_from_disk(&ichdr2, leaf2);
	ASSERT(ichdr2.count == 0);
	args = state->args;

	trace_xfs_attr_leaf_rebalance(args);

	 
	swap = 0;
	if (xfs_attr3_leaf_order(blk1->bp, &ichdr1, blk2->bp, &ichdr2)) {
		struct xfs_da_state_blk	*tmp_blk;
		struct xfs_attr3_icleaf_hdr tmp_ichdr;

		tmp_blk = blk1;
		blk1 = blk2;
		blk2 = tmp_blk;

		 
		tmp_ichdr = ichdr1;
		ichdr1 = ichdr2;
		ichdr2 = tmp_ichdr;

		leaf1 = blk1->bp->b_addr;
		leaf2 = blk2->bp->b_addr;
		swap = 1;
	}

	 
	state->inleaf = xfs_attr3_leaf_figure_balance(state, blk1, &ichdr1,
						      blk2, &ichdr2,
						      &count, &totallen);
	if (swap)
		state->inleaf = !state->inleaf;

	 
	if (count < ichdr1.count) {
		 
		 
		count = ichdr1.count - count;
		space  = ichdr1.usedbytes - totallen;
		space += count * sizeof(xfs_attr_leaf_entry_t);

		 
		max  = ichdr2.firstused - xfs_attr3_leaf_hdr_size(leaf1);
		max -= ichdr2.count * sizeof(xfs_attr_leaf_entry_t);
		if (space > max)
			xfs_attr3_leaf_compact(args, &ichdr2, blk2->bp);

		 
		xfs_attr3_leaf_moveents(leaf1, &ichdr1, ichdr1.count - count,
				leaf2, &ichdr2, 0, count, state->mp);

	} else if (count > ichdr1.count) {
		 
		ASSERT(0);

		 
		 
		count -= ichdr1.count;
		space  = totallen - ichdr1.usedbytes;
		space += count * sizeof(xfs_attr_leaf_entry_t);

		 
		max  = ichdr1.firstused - xfs_attr3_leaf_hdr_size(leaf1);
		max -= ichdr1.count * sizeof(xfs_attr_leaf_entry_t);
		if (space > max)
			xfs_attr3_leaf_compact(args, &ichdr1, blk1->bp);

		 
		xfs_attr3_leaf_moveents(leaf2, &ichdr2, 0, leaf1, &ichdr1,
					ichdr1.count, count, state->mp);
	}

	xfs_attr3_leaf_hdr_to_disk(leaf1, &ichdr1);
	xfs_attr3_leaf_hdr_to_disk(leaf2, &ichdr2);
	xfs_trans_log_buf(args->trans, blk1->bp, 0, state->blocksize-1);
	xfs_trans_log_buf(args->trans, blk2->bp, 0, state->blocksize-1);

	 
	entries1 = xfs_attr3_leaf_entryp(leaf1);
	entries2 = xfs_attr3_leaf_entryp(leaf2);
	blk1->hashval = be32_to_cpu(entries1[ichdr1.count - 1].hashval);
	blk2->hashval = be32_to_cpu(entries2[ichdr2.count - 1].hashval);

	 
	if (blk1->index > ichdr1.count) {
		ASSERT(state->inleaf == 0);
		blk2->index = blk1->index - ichdr1.count;
		args->index = args->index2 = blk2->index;
		args->blkno = args->blkno2 = blk2->blkno;
	} else if (blk1->index == ichdr1.count) {
		if (state->inleaf) {
			args->index = blk1->index;
			args->blkno = blk1->blkno;
			args->index2 = 0;
			args->blkno2 = blk2->blkno;
		} else {
			 
			blk2->index = blk1->index - ichdr1.count;
			args->index = blk2->index;
			args->blkno = blk2->blkno;
			if (!state->extravalid) {
				 
				args->index2 = blk2->index;
				args->blkno2 = blk2->blkno;
			}
		}
	} else {
		ASSERT(state->inleaf == 1);
		args->index = args->index2 = blk1->index;
		args->blkno = args->blkno2 = blk1->blkno;
	}
}
