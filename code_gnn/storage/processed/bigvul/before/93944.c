xfs_do_writepage(
	struct page		*page,
	struct writeback_control *wbc,
	void			*data)
{
	struct xfs_writepage_ctx *wpc = data;
	struct inode		*inode = page->mapping->host;
	loff_t			offset;
	__uint64_t              end_offset;
	pgoff_t                 end_index;

	trace_xfs_writepage(inode, page, 0, 0);

	ASSERT(page_has_buffers(page));

	 
	if (WARN_ON_ONCE((current->flags & (PF_MEMALLOC|PF_KSWAPD)) ==
			PF_MEMALLOC))
		goto redirty;

	 
	if (WARN_ON_ONCE(current->flags & PF_FSTRANS))
		goto redirty;

	 
	offset = i_size_read(inode);
	end_index = offset >> PAGE_SHIFT;
	if (page->index < end_index)
		end_offset = (xfs_off_t)(page->index + 1) << PAGE_SHIFT;
	else {
		 
		unsigned offset_into_page = offset & (PAGE_SIZE - 1);

		 
		if (page->index > end_index ||
		    (page->index == end_index && offset_into_page == 0))
			goto redirty;

		 
		zero_user_segment(page, offset_into_page, PAGE_SIZE);

		 
		end_offset = offset;
	}

	return xfs_writepage_map(wpc, wbc, inode, page, offset, end_offset);

redirty:
	redirty_page_for_writepage(wbc, page);
	unlock_page(page);
	return 0;
}
