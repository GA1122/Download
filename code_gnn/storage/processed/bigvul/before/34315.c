static void btrfs_invalidatepage(struct page *page, unsigned long offset)
{
	struct inode *inode = page->mapping->host;
	struct extent_io_tree *tree;
	struct btrfs_ordered_extent *ordered;
	struct extent_state *cached_state = NULL;
	u64 page_start = page_offset(page);
	u64 page_end = page_start + PAGE_CACHE_SIZE - 1;

	 
	wait_on_page_writeback(page);

	tree = &BTRFS_I(inode)->io_tree;
	if (offset) {
		btrfs_releasepage(page, GFP_NOFS);
		return;
	}
	lock_extent_bits(tree, page_start, page_end, 0, &cached_state);
	ordered = btrfs_lookup_ordered_extent(inode,
					   page_offset(page));
	if (ordered) {
		 
		clear_extent_bit(tree, page_start, page_end,
				 EXTENT_DIRTY | EXTENT_DELALLOC |
				 EXTENT_LOCKED | EXTENT_DO_ACCOUNTING |
				 EXTENT_DEFRAG, 1, 0, &cached_state, GFP_NOFS);
		 
		if (TestClearPagePrivate2(page) &&
		    btrfs_dec_test_ordered_pending(inode, &ordered, page_start,
						   PAGE_CACHE_SIZE, 1)) {
			btrfs_finish_ordered_io(ordered);
		}
		btrfs_put_ordered_extent(ordered);
		cached_state = NULL;
		lock_extent_bits(tree, page_start, page_end, 0, &cached_state);
	}
	clear_extent_bit(tree, page_start, page_end,
		 EXTENT_LOCKED | EXTENT_DIRTY | EXTENT_DELALLOC |
		 EXTENT_DO_ACCOUNTING | EXTENT_DEFRAG, 1, 1,
		 &cached_state, GFP_NOFS);
	__btrfs_releasepage(page, GFP_NOFS);

	ClearPageChecked(page);
	if (PagePrivate(page)) {
		ClearPagePrivate(page);
		set_page_private(page, 0);
		page_cache_release(page);
	}
}
