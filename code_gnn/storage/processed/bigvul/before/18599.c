static int ext4_find_delalloc_range(struct inode *inode,
				    ext4_lblk_t lblk_start,
				    ext4_lblk_t lblk_end,
				    int search_hint_reverse)
{
	struct address_space *mapping = inode->i_mapping;
	struct buffer_head *head, *bh = NULL;
	struct page *page;
	ext4_lblk_t i, pg_lblk;
	pgoff_t index;

	if (!test_opt(inode->i_sb, DELALLOC))
		return 0;

	 
	if (inode->i_blkbits < PAGE_CACHE_SHIFT)
		search_hint_reverse = 0;

	if (search_hint_reverse)
		i = lblk_end;
	else
		i = lblk_start;

	index = i >> (PAGE_CACHE_SHIFT - inode->i_blkbits);

	while ((i >= lblk_start) && (i <= lblk_end)) {
		page = find_get_page(mapping, index);
		if (!page)
			goto nextpage;

		if (!page_has_buffers(page))
			goto nextpage;

		head = page_buffers(page);
		if (!head)
			goto nextpage;

		bh = head;
		pg_lblk = index << (PAGE_CACHE_SHIFT -
						inode->i_blkbits);
		do {
			if (unlikely(pg_lblk < lblk_start)) {
				 
				pg_lblk++;
				continue;
			}

			 
			if (buffer_delay(bh) && !buffer_da_mapped(bh)) {
				page_cache_release(page);
				trace_ext4_find_delalloc_range(inode,
						lblk_start, lblk_end,
						search_hint_reverse,
						1, i);
				return 1;
			}
			if (search_hint_reverse)
				i--;
			else
				i++;
		} while ((i >= lblk_start) && (i <= lblk_end) &&
				((bh = bh->b_this_page) != head));
nextpage:
		if (page)
			page_cache_release(page);
		 
		if (search_hint_reverse)
			index--;
		else
			index++;
		i = index << (PAGE_CACHE_SHIFT - inode->i_blkbits);
	}

	trace_ext4_find_delalloc_range(inode, lblk_start, lblk_end,
					search_hint_reverse, 0, 0);
	return 0;
}