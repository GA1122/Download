static int lock_extent_direct(struct inode *inode, u64 lockstart, u64 lockend,
			      struct extent_state **cached_state, int writing)
{
	struct btrfs_ordered_extent *ordered;
	int ret = 0;

	while (1) {
		lock_extent_bits(&BTRFS_I(inode)->io_tree, lockstart, lockend,
				 0, cached_state);
		 
		ordered = btrfs_lookup_ordered_range(inode, lockstart,
						     lockend - lockstart + 1);

		 
		if (!ordered && (!writing ||
		    !test_range_bit(&BTRFS_I(inode)->io_tree,
				    lockstart, lockend, EXTENT_UPTODATE, 0,
				    *cached_state)))
			break;

		unlock_extent_cached(&BTRFS_I(inode)->io_tree, lockstart, lockend,
				     cached_state, GFP_NOFS);

		if (ordered) {
			btrfs_start_ordered_extent(inode, ordered, 1);
			btrfs_put_ordered_extent(ordered);
		} else {
			 
			ret = filemap_write_and_wait_range(inode->i_mapping,
							   lockstart,
							   lockend);
			if (ret)
				break;

			 
			ret = invalidate_inode_pages2_range(inode->i_mapping,
					lockstart >> PAGE_CACHE_SHIFT,
					lockend >> PAGE_CACHE_SHIFT);
			if (ret)
				break;
		}

		cond_resched();
	}

	return ret;
}
