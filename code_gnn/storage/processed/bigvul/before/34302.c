static int btrfs_get_blocks_direct(struct inode *inode, sector_t iblock,
				   struct buffer_head *bh_result, int create)
{
	struct extent_map *em;
	struct btrfs_root *root = BTRFS_I(inode)->root;
	struct extent_state *cached_state = NULL;
	u64 start = iblock << inode->i_blkbits;
	u64 lockstart, lockend;
	u64 len = bh_result->b_size;
	struct btrfs_trans_handle *trans;
	int unlock_bits = EXTENT_LOCKED;
	int ret;

	if (create) {
		ret = btrfs_delalloc_reserve_space(inode, len);
		if (ret)
			return ret;
		unlock_bits |= EXTENT_DELALLOC | EXTENT_DIRTY;
	} else {
		len = min_t(u64, len, root->sectorsize);
	}

	lockstart = start;
	lockend = start + len - 1;

	 
	if (lock_extent_direct(inode, lockstart, lockend, &cached_state, create))
		return -ENOTBLK;

	if (create) {
		ret = set_extent_bit(&BTRFS_I(inode)->io_tree, lockstart,
				     lockend, EXTENT_DELALLOC, NULL,
				     &cached_state, GFP_NOFS);
		if (ret)
			goto unlock_err;
	}

	em = btrfs_get_extent(inode, NULL, 0, start, len, 0);
	if (IS_ERR(em)) {
		ret = PTR_ERR(em);
		goto unlock_err;
	}

	 
	if (test_bit(EXTENT_FLAG_COMPRESSED, &em->flags) ||
	    em->block_start == EXTENT_MAP_INLINE) {
		free_extent_map(em);
		ret = -ENOTBLK;
		goto unlock_err;
	}

	 
	if (!create && (em->block_start == EXTENT_MAP_HOLE ||
			test_bit(EXTENT_FLAG_PREALLOC, &em->flags))) {
		free_extent_map(em);
		ret = 0;
		goto unlock_err;
	}

	 
	if (!create) {
		len = min(len, em->len - (start - em->start));
		lockstart = start + len;
		goto unlock;
	}

	if (test_bit(EXTENT_FLAG_PREALLOC, &em->flags) ||
	    ((BTRFS_I(inode)->flags & BTRFS_INODE_NODATACOW) &&
	     em->block_start != EXTENT_MAP_HOLE)) {
		int type;
		int ret;
		u64 block_start;

		if (test_bit(EXTENT_FLAG_PREALLOC, &em->flags))
			type = BTRFS_ORDERED_PREALLOC;
		else
			type = BTRFS_ORDERED_NOCOW;
		len = min(len, em->len - (start - em->start));
		block_start = em->block_start + (start - em->start);

		 
		trans = btrfs_join_transaction(root);
		if (IS_ERR(trans))
			goto must_cow;

		if (can_nocow_odirect(trans, inode, start, len) == 1) {
			u64 orig_start = em->orig_start;
			u64 orig_block_len = em->orig_block_len;

			if (type == BTRFS_ORDERED_PREALLOC) {
				free_extent_map(em);
				em = create_pinned_em(inode, start, len,
						       orig_start,
						       block_start, len,
						       orig_block_len, type);
				if (IS_ERR(em)) {
					btrfs_end_transaction(trans, root);
					goto unlock_err;
				}
			}

			ret = btrfs_add_ordered_extent_dio(inode, start,
					   block_start, len, len, type);
			btrfs_end_transaction(trans, root);
			if (ret) {
				free_extent_map(em);
				goto unlock_err;
			}
			goto unlock;
		}
		btrfs_end_transaction(trans, root);
	}
must_cow:
	 
	len = bh_result->b_size;
	free_extent_map(em);
	em = btrfs_new_extent_direct(inode, start, len);
	if (IS_ERR(em)) {
		ret = PTR_ERR(em);
		goto unlock_err;
	}
	len = min(len, em->len - (start - em->start));
unlock:
	bh_result->b_blocknr = (em->block_start + (start - em->start)) >>
		inode->i_blkbits;
	bh_result->b_size = len;
	bh_result->b_bdev = em->bdev;
	set_buffer_mapped(bh_result);
	if (create) {
		if (!test_bit(EXTENT_FLAG_PREALLOC, &em->flags))
			set_buffer_new(bh_result);

		 
		if (start + len > i_size_read(inode))
			i_size_write(inode, start + len);
	}

	 
	if (lockstart < lockend) {
		if (create && len < lockend - lockstart) {
			clear_extent_bit(&BTRFS_I(inode)->io_tree, lockstart,
					 lockstart + len - 1,
					 unlock_bits | EXTENT_DEFRAG, 1, 0,
					 &cached_state, GFP_NOFS);
			 
			clear_extent_bit(&BTRFS_I(inode)->io_tree,
					 lockstart + len, lockend,
					 unlock_bits | EXTENT_DO_ACCOUNTING |
					 EXTENT_DEFRAG, 1, 0, NULL, GFP_NOFS);
		} else {
			clear_extent_bit(&BTRFS_I(inode)->io_tree, lockstart,
					 lockend, unlock_bits, 1, 0,
					 &cached_state, GFP_NOFS);
		}
	} else {
		free_extent_state(cached_state);
	}

	free_extent_map(em);

	return 0;

unlock_err:
	if (create)
		unlock_bits |= EXTENT_DO_ACCOUNTING;

	clear_extent_bit(&BTRFS_I(inode)->io_tree, lockstart, lockend,
			 unlock_bits, 1, 0, &cached_state, GFP_NOFS);
	return ret;
}
